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
	
	void DisplayAmmo();
	
	bool EnoughAmmo(int AmmoNeeded);
	void UseAmmo(int AmmoUsed);

	void Reload();
	void MakeReload();
	int GetMaxAmmo() const { return MaxAmmo; }
	int GetCurrentAmmo() const { return CurrentAmmo; }
	bool GetIsReloading() const { return IsReloading; }
	void AddAmmo(int AmmoToAdd);
	bool IsFull() const { return CurrentAmmo == MaxAmmo; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	int MaxAmmo = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	int CurrentAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	int CurrentAmmoInMag = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	int MagCapacity = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	float ReloadTime = 2.f;
	
	FTimerHandle ReloadTimerHandler;

	float RemainingReloadTime = 0.f;
	bool IsReloading = false;
	
};
