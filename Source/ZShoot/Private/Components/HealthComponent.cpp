// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "GameInstances/GILevel.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/AIZombie.h"
#include "Pawns/PlayerActor.h"
#include "Widgets/PlayerHUDWidget.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
	
	APlayerActor* PlayerActor = Cast<APlayerActor>(GetOwner());
	if (PlayerActor)
	{
		PlayerActor->PlayerHUD->SetHealthValue(CurrentHealth / MaxHealth);
	}
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Die();
	}

	APlayerActor* PlayerActor = Cast<APlayerActor>(GetOwner());
	if (PlayerActor)
	{
		PlayerActor->PlayerHUD->SetHealthValue(CurrentHealth / MaxHealth);
	}
}

void UHealthComponent::Die()
{
	if (APlayerActor* PlayerActor = Cast<APlayerActor>(GetOwner()))
	{
		if (UGILevel* GameInstance = Cast<UGILevel>(UGameplayStatics::GetGameInstance(this)))
		{
			GameInstance->RestartLevel();
			return;
		}
	}

	if (AAIZombie* EnemyActor = Cast<AAIZombie>(GetOwner()))
	{
		if (UGILevel* GameInstance = Cast<UGILevel>(UGameplayStatics::GetGameInstance(this)))
		{
			GameInstance->OnEnemyDeath();
		}
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UHealthComponent::DestroyOwner);
}

void UHealthComponent::DestroyOwner()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->Destroy();
	}
}

