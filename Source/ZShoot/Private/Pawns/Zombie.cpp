#include "Pawns/Zombie.h"

#include "NavigationSystem.h"
#include "Components/HealthComponent.h"
#include "Pawns/PlayerClass.h"
#include "PowerUp/PowerUpHeal.h"
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

	PlayerActor = Cast<APlayerClass>(UGameplayStatics::GetPlayerPawn(this, 0));

	AIController = Cast<AAIController>(GetController());

	CharacterMovement = GetCharacterMovement();

	CharacterMovement->MaxWalkSpeed = WalkingSpeed;

	SetState(EZombieState::Idle);

	BodyMesh = GetMesh();

	UpdateState();
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateState();
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

void AZombie::SpawnPowerUp()
{
	UWorld* World = GetWorld();
	if (!World) return;

	float RandomValue = FMath::RandRange(0.f, 1.f);
	TSubclassOf<AActor> SelectedPowerUpClass = nullptr;

	if (RandomValue <= AmmoSpawnChance && PowerUpAmmoClass)
	{
		SelectedPowerUpClass = PowerUpAmmoClass;
	}
	else if (RandomValue <= (AmmoSpawnChance + HealSpawnChance) && PowerUpHealClass)
	{
		SelectedPowerUpClass = PowerUpHealClass;

		APowerUpHeal* SpawnedHeal = World->SpawnActor<APowerUpHeal>(SelectedPowerUpClass, GetActorLocation() + FVector(0.f, 0.f, 50.f), FRotator::ZeroRotator);
		if (SpawnedHeal)
		{
			SpawnedHeal->SetRandomHealType();  // Randomly select a heal type
		}
	}

	if (SelectedPowerUpClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		World->SpawnActor<AActor>(SelectedPowerUpClass, SpawnLocation, SpawnRotation);
	}
}


void AZombie::ChangeAnimation()
{
	if (CurrentAnimation != LastAnimation)
	{
		LastAnimation = CurrentAnimation;
		BodyMesh->PlayAnimation(Animations[CurrentAnimation], true);
	}
}

// Update the current state
void AZombie::UpdateState()
{
	CheckChasingRange();
	CheckInAttackRange();
	switch (CurrentState)
	{
	case EZombieState::Idle:
		HandleIdleState();
		break;
	case EZombieState::Walking:
		HandleWalkingState();
		break;
	case EZombieState::Chase:
		HandleChaseState();
		break;
	case EZombieState::Attack:
		HandleAttackState();
		break;
	default:
		break;
	}
}

// --------- Idle ---------

// Manage the idle state
void AZombie::HandleIdleState()
{
	CurrentAnimation = EZombieAnimation::Idle;
	ChangeAnimation();
	
	if (!IsWalking)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			SetState(EZombieState::Walking);
		});
		StateTimerHandler.Invalidate();
		GetWorldTimerManager().SetTimer(StateTimerHandler, TimerDelegate, FMath::RandRange(1.f, 5.f), false);
	}
}

// --------- Wander ---------

// Manage the wander state
void AZombie::HandleWalkingState()
{
	SetState(EZombieState::Walking);

	if (AIController && !IsWalking)
	{
		CurrentAnimation = EZombieAnimation::Walking;

		ChangeAnimation();

		CharacterMovement->MaxWalkSpeed = WalkingSpeed;
		FVector WanderLocation = GetRandomPointInNavigableRadius();
		IsWalking = true;
		FAIRequestID RequestID = AIController->MoveToLocation(WanderLocation, -1, true, true, true, true, 0, true);
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AZombie::OnWanderCompleted);
	}
}

// Called when the wander state is completed
void AZombie::OnWanderCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		IsWalking = false;
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
	if (AIController)
	{
		CurrentAnimation = EZombieAnimation::Chase;

		ChangeAnimation();

		CharacterMovement->MaxWalkSpeed = ChasingSpeed;
		FAIRequestID RequestID = AIController->MoveToActor(PlayerActor->GetController(), AttackRange / 2.f, true, true, true);
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AZombie::OnChasingCompleted);
	}
}

// Called when the chasing state is completed
void AZombie::OnChasingCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		SetState(EZombieState::Attack);
	}
}

// Returns true if the player is in chasing range
void AZombie::CheckChasingRange()
{
	float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());

	if (Distance <= ChasingRange)
	{
		SetState(EZombieState::Chase);
	}	
}

bool AZombie::CheckInAttackRange()
{
	float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());

	if (Distance < AttackRange)
	{
		SetState(EZombieState::Attack);
		return true;
	}
	
	return false;
}

void AZombie::HandleAttackState()
{
	if (PlayerActor && CanAttack)
	{
		CurrentAnimation = EZombieAnimation::Attack;
		ChangeAnimation();
		
		auto DamageTypeClass = UDamageType::StaticClass();		
		UGameplayStatics::ApplyDamage(PlayerActor, AttackDamage, GetInstigatorController(), this, DamageTypeClass);
		SetState(EZombieState::Idle);
		CanAttack = false;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
		{
			CanAttack = true;
		});
		AttackTimerHandler.Invalidate();
		GetWorldTimerManager().SetTimer(AttackTimerHandler, TimerDelegate, AttackInterval, false);
	}
}
