// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ammo.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZSHOOT_API UAmmo : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAmmo();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool EnoughAmmo(int AmmoNeeded) const { return CurrentAmmo >= AmmoNeeded; }
	void UseAmmo(int AmmoUsed) { CurrentAmmo -= AmmoUsed; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	int MaxAmmo = 100;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	int CurrentAmmo = 0;
	
};
