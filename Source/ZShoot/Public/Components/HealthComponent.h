// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSHOOT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Heal(float HealAmount);
	bool IsFullHealth() const { return CurrentHealth == MaxHealth; }
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Category = "Components")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, Category = "Components")
	float CurrentHealth = 0.f;

	// Functions to process damage and death
	
	void Die();
	void DestroyOwner();	
};
