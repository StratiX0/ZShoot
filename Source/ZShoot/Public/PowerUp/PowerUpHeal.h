// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpHeal.generated.h"

UCLASS()
class ZSHOOT_API APowerUpHeal : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerUpHeal();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RingMesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
    float AnimationHeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float AnimationSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Heal Properties", meta = (AllowPrivateAccess = "true"))
	float HealAmount = 25.f;
};
