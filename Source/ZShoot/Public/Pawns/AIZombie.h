#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIZombie.generated.h"

class UBoxComponent;
class UHealthComponent;
class APlayerActor;

UCLASS()
class ZSHOOT_API AAIZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AAIZombie();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	// Model Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<USkeletalMesh> SkeletalMeshAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	APlayerActor* PlayerActor;

	// Movement Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float Speed = 200.f;

	// --------------------------------------- Combat Properties ---------------------------------------

	// Chasing Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingRange = 1000.f;
	bool InChasingRange();
	void Chasing();

	// Attacking Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 1000.f;
	bool CanAttack = true;
	bool InAttackRange();
	void Attack();
	void AllowAttack();
	FTimerHandle AttackTimerHandler;
};