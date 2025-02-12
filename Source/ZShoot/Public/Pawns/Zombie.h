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

UCLASS()
class ZSHOOT_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	AAIController* AIController;
	UCharacterMovementComponent* CharacterMovement;

	APlayerActor* PlayerActor;

	// ---------------------------------- State Properties ----------------------------------
	
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
	EZombieState CurrentState;

	FTimerHandle StateTimerHandler;
	void SetState(EZombieState NewState);
	
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
};