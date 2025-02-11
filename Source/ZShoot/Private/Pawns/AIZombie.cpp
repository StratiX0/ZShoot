#include "Pawns/AIZombie.h"
#include "Components/HealthComponent.h"
#include "Pawns/PlayerActor.h"
#include "Kismet/GameplayStatics.h"

AAIZombie::AAIZombie()
{
 PrimaryActorTick.bCanEverTick = true;


 SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
 RootComponent = SkeletalMeshComponent;

 SkeletalMeshComponent->SetSkeletalMesh(SkeletalMeshAsset.Get());

 HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AAIZombie::BeginPlay()
{
 Super::BeginPlay();

 PlayerActor = Cast<APlayerActor>(UGameplayStatics::GetPlayerPawn(this, 0));

 GetWorldTimerManager().SetTimer(AttackTimerHandler, this, &AAIZombie::AllowAttack, AttackRate, true);
}

void AAIZombie::Tick(float DeltaTime)
{
 Super::Tick(DeltaTime);

 if (InChasingRange())
 {
  Chasing();
 }
}

bool AAIZombie::InChasingRange()
{
 if (PlayerActor)
 {
  float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());
  if (Distance <= ChasingRange)
  {
   return true;
  }
 }
 return false;
}

void AAIZombie::Chasing()
{
 if (PlayerActor)
 {
  FVector DeltaLocation = PlayerActor->GetActorLocation() - GetActorLocation();
  DeltaLocation = DeltaLocation.GetSafeNormal() * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
  DeltaLocation.Z = 0.f;
  AddActorLocalOffset(DeltaLocation, true);
 }
}

bool AAIZombie::InAttackRange()
{
 if (PlayerActor)
 {
  float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());
  if (Distance <= AttackRange)
  {
   return true;
  }
 }
 return false;
}

void AAIZombie::Attack()
{
 if (!CanAttack) return;

 if (PlayerActor)
 {
  auto DamageTypeClass = UDamageType::StaticClass();
  UGameplayStatics::ApplyDamage(PlayerActor, Damage, GetInstigatorController(), this, DamageTypeClass);
  GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, PlayerActor->GetName());
 }
 CanAttack = false;
}

void AAIZombie::AllowAttack()
{
 CanAttack = true;
 if (InAttackRange())
 {
  Attack();
 }
}