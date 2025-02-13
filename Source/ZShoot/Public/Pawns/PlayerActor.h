#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/Ammo.h"
#include "PlayerActor.generated.h"

class UPlayerHUDWidget;
class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
class USoundCue;
class UHealthComponent;
class UAmmo;

/**
 * APlayerActor - Main player character class for handling input, movement, camera switching, combat, etc.
 */
UCLASS()
class ZSHOOT_API APlayerActor : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerActor();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	// HUD Properties
	UPROPERTY(EditAnywhere, Category = "HUD")
	UPlayerHUDWidget* PlayerHUD;

	// Component Properties
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "Ammo")
	UAmmo* AmmoComponent;

protected:
	virtual void BeginPlay() override;

	// Input Action Properties
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchCameraSideAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;

	// Movement and Combat Functions
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void SwitchCameraSide(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Feedback")
	void PlayCameraShake();

private:
	// Model Components (Removed BlueprintReadOnly and kept private)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WheelMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* ShootingPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	// Movement Properties
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InterpSpeed = 2.f;

	// Camera Properties
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CamSens = 1.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraOffsetY = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraSwitchTime = 1.f;

	// Combat Properties
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 500.f;

	// Cooldown Flags
	bool CanSwitchCameraSide = true;
	bool CameraIsOnRightSide = true;
	bool CanShoot = true;

	FTimerHandle CameraSwitchTimerHandler;
	FTimerHandle ShootTimerHandler;

	// VFX & Audio Properties
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* MuzzleFlashVFX;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* BloodSplashVFX;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* FireSFX;

	UPROPERTY(EditAnywhere, Category = "Feedback")
	TSubclassOf<AActor> PinActor;

	UPROPERTY(EditAnywhere, Category = "Feedback")
	float PinVelocity = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Feedback")
	float PinTimer = 0.5f;

	// Internal Helper Functions
	void BindInputAction(UEnhancedInputComponent* InputComponent, UInputAction* Action, void (APlayerActor::*ActionFunc)(const FInputActionValue&));
	void RotateWheels(const FVector& DeltaLocation);
	void StartShootCooldown();
	void PlayFireEffects();
	void SpawnBloodSplashEffect(const FHitResult& HitResult);
	void ApplyDamageToActor(const FHitResult& HitResult);
	void AllowCameraSwitch() { CanSwitchCameraSide = true; }
	void AllowShoot() { CanShoot = true; }
	void SpawnPins();

	// Raycast for shooting
	FHitResult FireRaycast();

	UPROPERTY()
	APlayerController* PlayerController;
};
