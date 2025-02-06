// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/PlayerActor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathUtil.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Pawns/AIZombie.h"
#include "Components/Ammo.h"

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel Mesh"));
	WheelMesh->SetupAttachment(BoxComp);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BodyMesh->SetupAttachment(BoxComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	ShootingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ShootingPoint->SetupAttachment(TurretMesh);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(BoxComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	AmmoComponent = CreateDefaultSubobject<UAmmo>(TEXT("Ammo Component"));

}

void APlayerActor::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementAction)
		{
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerActor::Move);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerActor::LookAround);
		}
		if (SwitchCameraSideAction)
		{
			EnhancedInputComponent->BindAction(SwitchCameraSideAction, ETriggerEvent::Triggered, this, &APlayerActor::SwitchCameraSide);
		}
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerActor::Fire);
		}
	}
}

void APlayerActor::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// Global Movement
		FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
		SpringArmRotation.Pitch = 0.f;
		FVector SpringArmForwardDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::X);
		FVector SpringArmRightDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::Y);

		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation += SpringArmForwardDirection * MovementVector.Y * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		DeltaLocation += SpringArmRightDirection * MovementVector.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		DeltaLocation.Z = 0.f;
		AddActorLocalOffset(DeltaLocation, true);

		// Wheel Movement, fake movement
		FVector RotationAxis = FVector::CrossProduct(FVector::UpVector, DeltaLocation.GetSafeNormal());

		float Distance = DeltaLocation.Size();
		float SphereRadius = WheelMesh->GetComponentScale().X * 50.f;
		float RollAngle = FMath::RadiansToDegrees(Distance / SphereRadius);

		FQuat RotationDelta = FQuat(RotationAxis, FMath::DegreesToRadians(RollAngle));
		WheelMesh->AddLocalRotation(RotationDelta);

	}
}

void APlayerActor::LookAround(const FInputActionValue& Value)
{
	const FVector2D RotationVector = Value.Get<FVector2D>();
	
	if (Controller)
	{
		// Turret Rotation
		FRotator TurretDeltaRotation = FRotator::ZeroRotator;
		FRotator TurretRotator = TurretMesh->GetRelativeRotation();
		TurretDeltaRotation.Yaw = TurretRotator.Yaw + RotationVector.X * CamSens * UGameplayStatics::GetWorldDeltaSeconds(this);
		TurretMesh->SetWorldRotation(TurretDeltaRotation, false);

		// Spring Arm Rotation -> Camera Rotation
		FRotator SpringArmDeltaRotation = FRotator::ZeroRotator;
		FRotator SpringArmRotator = SpringArmComp->GetRelativeRotation();
		SpringArmDeltaRotation.Yaw = SpringArmRotator.Yaw + RotationVector.X * CamSens * UGameplayStatics::GetWorldDeltaSeconds(this);
		SpringArmDeltaRotation.Pitch = SpringArmRotator.Pitch + RotationVector.Y * CamSens * UGameplayStatics::GetWorldDeltaSeconds(this);
		SpringArmDeltaRotation.Pitch = FMathf::Clamp(SpringArmDeltaRotation.Pitch, -89.9f, 89.9f);
		SpringArmComp->SetWorldRotation(SpringArmDeltaRotation, false);
	}
}

void APlayerActor::SwitchCameraSide(const FInputActionValue& Value)
{
	if (!CanSwitchCameraSide) return;

	CameraIsOnRightSide = !CameraIsOnRightSide;
	
	CameraIsOnRightSide ? CameraOffsetY = 100.f : CameraOffsetY = -100.f;

	SpringArmComp->SocketOffset = FVector(0.f, CameraOffsetY, SpringArmComp->SocketOffset.Z);

	CanSwitchCameraSide = false;
	CameraSwitchTimerHandler.Invalidate();
	GetWorldTimerManager().SetTimer(CameraSwitchTimerHandler, this, &APlayerActor::AllowCameraSwitch, CameraSwitchTime, false);
}

void APlayerActor::Fire(const FInputActionValue& Value)
{
	if (!CanShoot || !AmmoComponent->EnoughAmmo(1)) return;
	
	FHitResult OutHit = FireRaycast();

	if (ShootingVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ShootingVFX, ShootingPoint->GetComponentLocation(), ShootingPoint->GetComponentRotation());
	}

	if (FireSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSFX,
			GetActorLocation());
	}
	
	if (OutHit.GetActor())
	{
		auto DamageTypeClass = UDamageType::StaticClass();		
		UGameplayStatics::ApplyDamage(OutHit.GetActor(), Damage, GetInstigatorController(), this, DamageTypeClass);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, OutHit.GetActor()->GetName());
		AAIZombie* Zombie = Cast<AAIZombie>(OutHit.GetActor());
		if (PlayerHUD && Zombie)
		{
			PlayerHUD->ShowHitMarker();
		}
	}

	AmmoComponent->UseAmmo(1);

	float Interval = 60.f / FireRate;

	ShootTimerHandler.Invalidate();
	GetWorldTimerManager().SetTimer(ShootTimerHandler, this, &APlayerActor::AllowShoot, Interval, false);
	CanShoot = false;
}

FHitResult APlayerActor::FireRaycast()
{
	FVector Start = CameraComp->GetComponentLocation();

	int distance = 999999999;
	FVector End = CameraComp->GetComponentLocation() + (CameraComp->GetForwardVector() * distance);

	DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), true, 5, 0, 0.7f);

	FHitResult OutHit;
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	
	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, RV_TraceParams);

	return OutHit;
}

