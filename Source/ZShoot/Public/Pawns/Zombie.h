#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Zombie.generated.h"

class AAIController;
class UHealthComponent;
class APlayerActor;
class UPathFollowingComponent;

UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Chase UMETA(DisplayName = "Chase"),
	Wander UMETA(DisplayName = "Wander"),
	Attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EZombieAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle Animation"),
	Chase UMETA(DisplayName = "Chase Animation"),
	Wander UMETA(DisplayName = "Wander Animation"),
	Attack UMETA(DisplayName = "Attack Animation")
};

UCLASS()
class ZSHOOT_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetState(EZombieState NewState);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	AAIController* AIController;
	UCharacterMovementComponent* CharacterMovement;

	APlayerActor* PlayerActor;

	USkeletalMeshComponent* BodyMesh;

	// ---------------------------------- Animation Properties ----------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	EZombieAnimation CurrentAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	EZombieAnimation LastAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TMap<EZombieAnimation, UAnimSequence*> Animations;
	bool PlayingAnimation = false;

	void ChangeAnimation();
	

	// ---------------------------------- State Properties ----------------------------------
	
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
	EZombieState CurrentState;

	FTimerHandle StateTimerHandler;
	
	void UpdateState();
	void HandleIdleState();

	// Wander Properties
	
	void HandleWanderState();
	
	UFUNCTION()
	void OnWanderCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	FNavLocation GetRandomPointInNavigableRadius();
	bool IsWaitingToWander = false;
	bool IsWandering = false;

	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float WanderSpeed = 200.f;
	
	// Attack Properties
	
	void HandleAttackState();
	
	// Chasing Properties

	void HandleChaseState();
	UFUNCTION()
	void OnChasingCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	bool IsChasing = false;
	
	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingSpeed = 400.f;

	bool InChasingRange();
	void CheckChasing();

	// Attack Properties
	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.f;
};