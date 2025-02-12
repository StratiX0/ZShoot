#include "Pawns/Zombie.h"

#include "NavigationSystem.h"
#include "Components/HealthComponent.h"
#include "Pawns/PlayerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AZombie::AZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	PlayerActor = Cast<APlayerActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	AIController = Cast<AAIController>(GetController());

	CharacterMovement = GetCharacterMovement();

	CharacterMovement->MaxWalkSpeed = WanderSpeed;

	CurrentState = EZombieState::Idle;

	UpdateState();
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InChasingRange();
	UpdateState();
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Override TakeDamage function
float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->DamageTaken(this, DamageAmount, nullptr, EventInstigator, DamageCauser);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}





// ------------------------------ STATES ------------------------------

void AZombie::SetState(EZombieState NewState)
{
	CurrentState = NewState;
}

// Update the current state
void AZombie::UpdateState()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Current State: %s"), *UEnum::GetValueAsString(CurrentState)));
	switch (CurrentState)
	{
	case EZombieState::Idle:
		HandleIdleState();
		break;
	case EZombieState::Chase:
		HandleChaseState();
		break;
	case EZombieState::Wander:
		HandleWanderState();
		break;
	default:
		break;
	}
}

// --------- Idle ---------

// Manage the idle state
void AZombie::HandleIdleState()
{
	CheckChasing();
	if (!IsWandering)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			SetState(EZombieState::Wander);
		});
		StateTimerHandler.Invalidate();
		GetWorldTimerManager().SetTimer(StateTimerHandler, TimerDelegate, FMath::RandRange(1.f, 5.f), false);
	}
}

// --------- Wander ---------

// Manage the wander state
void AZombie::HandleWanderState()
{
	SetState(EZombieState::Wander);
	CheckChasing();
	
	if (AIController && !IsWandering)
	{
		CharacterMovement->MaxWalkSpeed = WanderSpeed;
		FVector WanderLocation = GetRandomPointInNavigableRadius();
		IsWandering = true;
		FAIRequestID RequestID = AIController->MoveToLocation(WanderLocation, -1, true, true, true, true, 0, true);
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AZombie::OnWanderCompleted);
	}
}

// Called when the wander state is completed
void AZombie::OnWanderCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		IsWandering = false;
		SetState(EZombieState::Idle);
	}
}

// Get a random point in navigable radius
FNavLocation AZombie::GetRandomPointInNavigableRadius()
{
	FVector Origin = GetActorLocation();
	FNavLocation RandomPoint;
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		NavSys->GetRandomPointInNavigableRadius(Origin, 10000.f, RandomPoint);
	}
	return RandomPoint;
}

// --------- Chase ---------

// Manage the chase state
void AZombie::HandleChaseState()
{
	if (!InChasingRange())
	{
		SetState(EZombieState::Idle);
	}
	else if (AIController)
	{
		CharacterMovement->MaxWalkSpeed = ChasingSpeed;
		FAIRequestID RequestID = AIController->MoveToActor(PlayerActor->GetController(), 200.f, true, true, true);
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AZombie::OnChasingCompleted);
	}
}

// Called when the chasing state is completed
void AZombie::OnChasingCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		SetState(EZombieState::Idle);
	}
}

// Returns true if the player is in chasing range
bool AZombie::InChasingRange()
{
	float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());

	if (Distance <= ChasingRange)
	{
		SetState(EZombieState::Chase);
		return true;
	}
	return false;	
}

// Change the State to Chase if the player is in chasing range
void AZombie::CheckChasing()
{
	if (InChasingRange())
	{
		SetState(EZombieState::Chase);
	}
}

void AZombie::HandleAttackState()
{
	SetState(EZombieState::Attack);
}
