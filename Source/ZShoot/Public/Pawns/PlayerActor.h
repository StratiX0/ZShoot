// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
 * 
 */
UCLASS()
class ZSHOOT_API APlayerActor : public APawn
{
	GENERATED_BODY()

public:
	APlayerActor();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	// HUD Properties
	UPROPERTY(EditAnywhere, Category="HUD Properties")
	UPlayerHUDWidget* PlayerHUD;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo Components", meta = (AllowPrivateAccess = "true"))
	UAmmo* AmmoComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Input")
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category= "Input")
	UInputAction* MovementAction;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category= "Input")
	UInputAction* SwitchCameraSideAction;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category= "Input")
	UInputAction* ReloadAction;

	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	
	void SwitchCameraSide(const FInputActionValue& Value);
	void AllowCameraSwitch() { CanSwitchCameraSide = true; }
	void AllowShoot() { CanShoot = true; }

	void Fire(const FInputActionValue& Value);
	FHitResult FireRaycast();

	void Reload(const FInputActionValue& Value);

private:

	// Model Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WheelMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ShootingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	// Movement Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;

	// Camera Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Properties", meta = (AllowPrivateAccess = "true"))
	float CamSens = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Properties", meta = (AllowPrivateAccess = "true"))
	float CameraOffsetY = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float CameraSwitchTime = 1.f;
	
	bool CanSwitchCameraSide = true;
	bool CameraIsOnRightSide = true;
	FTimerHandle CameraSwitchTimerHandler;

	// Shooting Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float FireRate = 500.f;

	bool CanShoot = true;
	FTimerHandle ShootTimerHandler;

	UPROPERTY(EditAnywhere, Category = "Combat Properties")
	USoundCue* FireSFX;

	// VFX Properties
	UPROPERTY(EditAnywhere, Category="VFX Properties")
	UNiagaraSystem* MuzzleFlashVFX;

	UPROPERTY(EditAnywhere, Category="Shoot Properties")
	UNiagaraSystem* BloodSplashVFX;
};
