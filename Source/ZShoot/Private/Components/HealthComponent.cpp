// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "GameInstances/GILevel.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerClass.h"
#include "Pawns/Zombie.h"
#include "Widgets/PlayerHUDWidget.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AZombie* Owner = Cast<AZombie>(GetOwner()))
    {
        float RandomMultiplier = FMath::RandRange(RandomMultiplierMin, RandomMultiplierMax);
        MaxHealth *= RandomMultiplier;
    }
    
    CurrentHealth = MaxHealth;
    
    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
    }
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.f || CurrentHealth == MaxHealth) return;

    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
    UpdateHealthUI();
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
    if (Damage <= 0.f || CurrentHealth <= 0.f) return;

    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
    UpdateHealthUI();
    PlayPLayerBloodSplash();

    if (CurrentHealth <= 0.f)
    {
        Die();
    }
}

void UHealthComponent::Die()
{
    if (AActor* Owner = GetOwner())
    {
        if (APlayerClass* PlayerActor = Cast<APlayerClass>(Owner))
        {
            if (UGILevel* GameInstance = Cast<UGILevel>(UGameplayStatics::GetGameInstance(this)))
            {
                GameInstance->EndGame();
                return;
            }
        }
        else if (AZombie* ZombieEnemy = Cast<AZombie>(Owner))
        {
            HandleEnemyDeath(ZombieEnemy);
            ZombieEnemy->SpawnPowerUp();
        }

        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UHealthComponent::DestroyOwner);
    }
}

void UHealthComponent::HandleEnemyDeath(AActor* EnemyActor)
{
    if (UGILevel* GameInstance = Cast<UGILevel>(UGameplayStatics::GetGameInstance(this)))
    {
        GameInstance->OnEnemyDeath();
    }
}

void UHealthComponent::PlayPLayerBloodSplash()
{
    if (AActor* Owner = GetOwner())
    {
        if (APlayerClass* PlayerActor = Cast<APlayerClass>(Owner))
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PlayerBloodSplashVFX, PlayerActor->GetActorLocation());
        }
    }
}

void UHealthComponent::DestroyOwner()
{
    if (AActor* Owner = GetOwner())
    {
        Owner->Destroy();
    }
}

void UHealthComponent::UpdateHealthUI()
{
    if (APlayerClass* PlayerActor = Cast<APlayerClass>(GetOwner()))
    {
        if (PlayerActor->PlayerHUD)
        {
            PlayerActor->PlayerHUD->SetHealthValue(CurrentHealth / MaxHealth);
        }
    }
}

