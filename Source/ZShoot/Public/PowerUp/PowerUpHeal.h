// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpHeal.generated.h"

// Enum to define healing options
UENUM(BlueprintType)
enum class EPUHealType : uint8
{
	None UMETA(DisplayName = "None"),
	Option1 UMETA(DisplayName = "25 Heal"),
	Option2 UMETA(DisplayName = "50 Heal"),
	Option3 UMETA(DisplayName = "100 Heal")
};

UCLASS()
class ZSHOOT_API APowerUpHeal : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerUpHeal();
	virtual void Tick(float DeltaTime) override;

	void SetRandomHealType();

protected:
	virtual void BeginPlay() override;

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RingMesh;

	// Materials
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterial*> MaterialsArray;

	// Heal properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal Properties", meta = (AllowPrivateAccess = "true"))
	EPUHealType HealType = EPUHealType::Option1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal Properties", meta = (AllowPrivateAccess = "true"))
	float HealAmount = 25.f;

	// Animation properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
    float AnimationHeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float AnimationSpeed = 5.f;

	// Helper function to set heal amount based on selected option
	void SetStartHealAmount();

	// Hit event
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float InitialHeight;
};

