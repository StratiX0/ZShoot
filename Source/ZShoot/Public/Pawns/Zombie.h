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
	Walking UMETA(DisplayName = "Walk"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EZombieAnimation : uint8
{
	Idle UMETA(DisplayName = "Idle Animation"),
	Walking UMETA(DisplayName = "Walk Animation"),
	Chase UMETA(DisplayName = "Chase Animation"),
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

	// Walking Properties
	
	void HandleWalkingState();
	
	UFUNCTION()
	void OnWanderCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	FNavLocation GetRandomPointInNavigableRadius();
	bool IsWaitingToWalk = false;
	bool IsWalking = false;

	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float WalkingSpeed = 200.f;
	
	// Chasing Properties

	void HandleChaseState();
	UFUNCTION()
	void OnChasingCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	bool IsChasing = false;
	
	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement Properties", meta = (AllowPrivateAccess = "true"))
	float ChasingSpeed = 400.f;

	void CheckChasingRange();
	bool CheckInAttackRange();

	// Attack Properties
	UPROPERTY(EditAnywhere, Category = "Attack Properties", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Attack Properties", meta = (AllowPrivateAccess = "true"))
	float AttackDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Attack Properties", meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 2.f;

	FTimerHandle AttackTimerHandler;
	bool CanAttack = true;

	void HandleAttackState();
};