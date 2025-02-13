#include "Pawns/PlayerActor.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Pawns/AIZombie.h"
#include "Components/Ammo.h"
#include "Pawns/Zombie.h"

// Sets default values
APlayerActor::APlayerActor()
{
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
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(PlayerContext, 0);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController is not valid in BeginPlay!"));
    }
}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Setup player input actions
void APlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        BindInputAction(EnhancedInputComponent, MovementAction, &APlayerActor::Move);
        BindInputAction(EnhancedInputComponent, LookAction, &APlayerActor::LookAround);
        BindInputAction(EnhancedInputComponent, SwitchCameraSideAction, &APlayerActor::SwitchCameraSide);
        BindInputAction(EnhancedInputComponent, FireAction, &APlayerActor::Fire);
        BindInputAction(EnhancedInputComponent, ReloadAction, &APlayerActor::Reload);
    }
}

// Bind actions to input component
void APlayerActor::BindInputAction(UEnhancedInputComponent* PlayerInputComponent, UInputAction* Action, void (APlayerActor::*ActionFunc)(const FInputActionValue&))
{
    if (Action)
    {
        PlayerInputComponent->BindAction(Action, ETriggerEvent::Triggered, this, ActionFunc);
    }
}

// Move the player
void APlayerActor::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
        SpringArmRotation.Pitch = 0.f;

        FVector ForwardDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(SpringArmRotation).GetUnitAxis(EAxis::Y);
        FVector DeltaLocation = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X) * Speed * GetWorld()->GetDeltaSeconds();
        DeltaLocation.Z = 0.f;

        AddActorLocalOffset(DeltaLocation, true);
        RotateWheels(DeltaLocation);
    }
}

// Rotate the wheels based on movement
void APlayerActor::RotateWheels(const FVector& DeltaLocation)
{
    FVector RotationAxis = FVector::CrossProduct(FVector::UpVector, DeltaLocation.GetSafeNormal());
    float Distance = DeltaLocation.Size();
    float SphereRadius = WheelMesh->GetComponentScale().X * 50.f;
    float RollAngle = FMath::RadiansToDegrees(Distance / SphereRadius);
    FQuat RotationDelta = FQuat(RotationAxis, FMath::DegreesToRadians(RollAngle));
    WheelMesh->AddLocalRotation(RotationDelta);
}

// Look around (Turret and Camera)
void APlayerActor::LookAround(const FInputActionValue& Value)
{
    const FVector2D RotationVector = Value.Get<FVector2D>();
    if (Controller)
    {
        FRotator TurretRotator = TurretMesh->GetRelativeRotation();
        TurretRotator.Yaw += RotationVector.X * CamSens * GetWorld()->GetDeltaSeconds();
        TurretMesh->SetWorldRotation(TurretRotator);

        FRotator SpringArmRotator = SpringArmComp->GetRelativeRotation();
        SpringArmRotator.Yaw += RotationVector.X * CamSens * GetWorld()->GetDeltaSeconds();
        SpringArmRotator.Pitch = FMath::Clamp(SpringArmRotator.Pitch + RotationVector.Y * CamSens * GetWorld()->GetDeltaSeconds(), -89.9f, 89.9f);
        SpringArmComp->SetWorldRotation(SpringArmRotator);
    }
}

// Switch camera side
void APlayerActor::SwitchCameraSide(const FInputActionValue& Value)
{
    if (CanSwitchCameraSide)
    {
        CameraOffsetY *= -1;
        SpringArmComp->SocketOffset.Y = CameraOffsetY;

        CanSwitchCameraSide = false;
        GetWorldTimerManager().SetTimer(CameraSwitchTimerHandler, this, &APlayerActor::AllowCameraSwitch, CameraSwitchTime, false);
    }
}

// Fire a shot
void APlayerActor::Fire(const FInputActionValue& Value)
{
    if (CanShoot && AmmoComponent->EnoughAmmo(1) && !AmmoComponent->GetIsReloading())
    {
        FHitResult OutHit = FireRaycast();

        PlayFireEffects();

        if (OutHit.GetActor())
        {
            ApplyDamageToActor(OutHit);
        }

        AmmoComponent->UseAmmo(1);
        StartShootCooldown();
    }
}

// Handle shooting cooldown
void APlayerActor::StartShootCooldown()
{
    float Interval = 60.f / FireRate;
    GetWorldTimerManager().SetTimer(ShootTimerHandler, this, &APlayerActor::AllowShoot, Interval, false);
    CanShoot = false;
}

// Fire raycast to check hit
FHitResult APlayerActor::FireRaycast()
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
void APlayerActor::PlayFireEffects()
{
    if (MuzzleFlashVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlashVFX, ShootingPoint->GetComponentLocation(), ShootingPoint->GetComponentRotation());
    }

    if (FireSFX)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSFX, GetActorLocation());
    }

    SpawnPins();

    PlayCameraShake();
}

void APlayerActor::SpawnPins()
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
void APlayerActor::ApplyDamageToActor(const FHitResult& HitResult)
{
    auto DamageTypeClass = UDamageType::StaticClass();
    UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigatorController(), this, DamageTypeClass);

    AAIZombie* Zombie = Cast<AAIZombie>(HitResult.GetActor());
    if (PlayerHUD && Zombie)
    {
        PlayerHUD->ShowHitMarker();
        SpawnBloodSplashEffect(HitResult);
    }
}

// Spawn blood splash effect
void APlayerActor::SpawnBloodSplashEffect(const FHitResult& HitResult)
{
    FFXSystemSpawnParameters Params;
    Params.WorldContextObject = this;
    Params.SystemTemplate = BloodSplashVFX;
    Params.Location = HitResult.Location;
    Params.Rotation = FRotationMatrix::MakeFromZ(HitResult.ImpactNormal).Rotator();
    Params.Scale = FVector::OneVector;

    UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(Params);
}

// Reload the ammo
void APlayerActor::Reload(const FInputActionValue& Value)
{
    AmmoComponent->MakeReload();
}
