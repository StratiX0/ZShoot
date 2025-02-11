// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIZombie.generated.h"

class UBoxComponent;
class UHealthComponent;
class APlayerActor;

UCLASS()
class ZSHOOT_API AAIZombie : public APawn
{
	GENERATED_BODY()

public:
	AAIZombie();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	// Model Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	APlayerActor* PlayerActor;

	// Movement Properties
	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float Speed = 200.f;

	// --------------------------------------- Combat Properties ---------------------------------------

	// Chasing Properties
	UPROPERTY(EditAnywhere, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingRange = 1000.f;
	bool InChasingRange();
	void Chasing();

	// Attacking Properties
	UPROPERTY(EditAnywhere, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.f;
	UPROPERTY(EditAnywhere, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRate = 1.f;
	UPROPERTY(EditAnywhere, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 1000.f;
	bool CanAttack = true;
	bool InAttackRange();
	void Attack();
	void AllowAttack();
	FTimerHandle AttackTimerHandler;

};
