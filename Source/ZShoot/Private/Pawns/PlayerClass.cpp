#include "Pawns/PlayerClass.h"
#include "Components/HealthComponent.h"
#include "Components/Ammo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawns/Zombie.h"
#include "Sound/SoundCue.h"
#include "Widgets/PlayerHUDWidget.h"

// Sets default values
APlayerClass::APlayerClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMovement = GetCharacterMovement();

	BodyMesh = GetMesh();

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	RifleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	RifleMesh->SetupAttachment(RootComponent);

	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ShootingPoint->SetupAttachment(RifleMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	AmmoComponent = CreateDefaultSubobject<UAmmo>(TEXT("Ammo Component"));
}

void APlayerClass::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APlayerClass::InitializePlayerController, 0.1f, true);
}

void APlayerClass::InitializePlayerController()
{
	PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
			GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not valid in InitializePlayerController!"));
	}
}

// Called every frame
void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BindInputAction(EnhancedInputComponent, MovementAction, &APlayerClass::Move);
		BindInputAction(EnhancedInputComponent, LookAction, &APlayerClass::LookAround);
		BindInputAction(EnhancedInputComponent, SwitchCameraSideAction, &APlayerClass::SwitchCameraSide);
		BindInputAction(EnhancedInputComponent, FireAction, &APlayerClass::Fire);
		BindInputAction(EnhancedInputComponent, ReloadAction, &APlayerClass::Reload);
		BindInputAction(EnhancedInputComponent, AimAction, &APlayerClass::Aim);
	}
}

void APlayerClass::BindInputAction(UEnhancedInputComponent* LocalInputComponent, UInputAction* Action, void(APlayerClass::* ActionFunc)(const FInputActionValue&))
{
	if (Action)
	{
		LocalInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, ActionFunc);
		LocalInputComponent->BindAction(Action, ETriggerEvent::Completed, this, ActionFunc);
	}
}

void APlayerClass::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (PlayerController && CharacterMovement)
	{
		// Normalize the movement vector
		FVector MovementDirection = FVector(MovementVector.Y, MovementVector.X, 0.f).GetSafeNormal();

		// Calculate the force to apply
		FVector Force = MovementDirection * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);	

		AddMovementInput(GetActorForwardVector() * Force.X);
		AddMovementInput(GetActorRightVector() * Force.Y);
	}
}

// Look around (Turret and Camera)
void APlayerClass::LookAround(const FInputActionValue& Value)
{
	const FVector2D RotationVector = Value.Get<FVector2D>();
	if (PlayerController)
	{
		FRotator SpringArmRotator = SpringArmComp->GetRelativeRotation();
		SpringArmRotator.Yaw += RotationVector.X * CamSens * GetWorld()->GetDeltaSeconds();
		SpringArmRotator.Pitch = FMath::Clamp(SpringArmRotator.Pitch + RotationVector.Y * CamSens * UGameplayStatics::GetWorldDeltaSeconds(this), -89.9f, 89.9f);
		FRotator CameraRotator = SpringArmComp->GetRelativeRotation();
		CameraRotator.Pitch = SpringArmRotator.Pitch;
		AddControllerYawInput(SpringArmRotator.Yaw);
		SpringArmComp->SetRelativeRotation(CameraRotator);
	}
}

void APlayerClass::SwitchCameraSide(const FInputActionValue& Value)
{
	if (CanSwitchCameraSide)
	{
		CameraOffsetY *= -1;
		SpringArmComp->SocketOffset.Y = CameraOffsetY;

		CanSwitchCameraSide = false;
		GetWorldTimerManager().SetTimer(CameraSwitchTimerHandler, this, &APlayerClass::AllowCameraSwitch, CameraSwitchTime, false);
	}
}

void APlayerClass::Fire(const FInputActionValue& Value)
{
	if (CanShoot && AmmoComponent->EnoughAmmo(1) && !AmmoComponent->GetIsReloading())
	{
		FHitResult OutHit = FireRaycast();

		PlayFireEffects(&OutHit);

		if (OutHit.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *OutHit.GetActor()->GetName()));
			ApplyDamageToActor(OutHit);
		}

		AmmoComponent->UseAmmo(1);
		StartShootCooldown();
	}
}

// Reload the ammo
void APlayerClass::Reload(const FInputActionValue& Value)
{
	AmmoComponent->MakeReload();
}

void APlayerClass::Aim(const FInputActionValue& Value)
{
	bool bIsAiming = Value.Get<bool>();
	float TargetFOV = bIsAiming ? CameraAimFOV : CameraDefaultFOV;
	CameraComp->SetFieldOfView(TargetFOV);
}

void APlayerClass::StartShootCooldown()
{
	float Interval = 60.f / FireRate;
	GetWorldTimerManager().SetTimer(ShootTimerHandler, this, &APlayerClass::AllowShoot, Interval, false);
	CanShoot = false;
}

// Fire raycast to check hit
FHitResult APlayerClass::FireRaycast()
{
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + (CameraComp->GetForwardVector() * 999999999);

	FHitResult OutHit;
	FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, RV_TraceParams);

	return OutHit;
}

// Play muzzle flash and sound
void APlayerClass::PlayFireEffects(FHitResult* OutHit)
{        
	FVector Start = ShootingPoint->GetComponentLocation();
	FVector End = OutHit->ImpactPoint;

	FVector Direction = End - Start;
	Direction.Normalize();

	FRotator TracerRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	if (MuzzleFlashVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlashVFX, Start, TracerRotation);
	}

	if (FireSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSFX, Start);
	}

	if (BulletTracerVFX)
	{


		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BulletTracerVFX, Start, TracerRotation);
	}

	SpawnPins();

	PlayCameraShake();
}

void APlayerClass::SpawnPins()
{
	if (PinActor)
	{
		FVector SpawnLocation = ShootingPoint->GetComponentLocation();
		FRotator SpawnRotation = ShootingPoint->GetComponentRotation();
        
		AActor* SpawnedPin = GetWorld()->SpawnActor<AActor>(PinActor->GetDefaultObject()->GetClass(), SpawnLocation, SpawnRotation);

		if (SpawnedPin)
		{
			UPrimitiveComponent* PinComponent = Cast<UPrimitiveComponent>(SpawnedPin->GetRootComponent());

			if (PinComponent && PinComponent->IsSimulatingPhysics())
			{
				FVector RandomVelocity = FVector(
					FMath::FRandRange(-50.f, 50.f),
					100.f,
					FMath::FRandRange(-75.f, 25.f)
				);

				// Normalise le vecteur de vélocité
				FVector NormalizedVelocity = RandomVelocity.GetSafeNormal();

				// Multiplie par la force souhaitée (par exemple, 1000.f)
				FVector FinalVelocity = NormalizedVelocity * PinVelocity;

				// Applique la vélocité calculée
				PinComponent->SetPhysicsLinearVelocity(FinalVelocity);

				// Définir un timer pour détruire l'acteur après 1 seconde
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SpawnedPin]()
				{
					if (SpawnedPin)
					{
						SpawnedPin->Destroy();
					}
				}, PinTimer, false);
			}
		}
	}
}

// Apply damage to the hit actor
void APlayerClass::ApplyDamageToActor(const FHitResult& HitResult)
{
	auto DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigatorController(), this, DamageTypeClass);
	AZombie* Zombie = Cast<AZombie>(HitResult.GetActor());
	if (PlayerHUD && Zombie)
	{
		PlayerHUD->ShowHitMarker();
		SpawnBloodSplashEffect(HitResult);
	}
}

// Spawn blood splash effect
void APlayerClass::SpawnBloodSplashEffect(const FHitResult& HitResult)
{
	FFXSystemSpawnParameters Params;
	Params.WorldContextObject = this;
	Params.SystemTemplate = BloodSplashVFX;
	Params.Location = HitResult.Location;
	Params.Rotation = FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).Rotator();
	Params.Scale = FVector::OneVector;

	UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(Params);
}

