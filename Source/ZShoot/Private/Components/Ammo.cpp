// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Ammo.h"
#include "Pawns/PlayerActor.h"
#include "Widgets/PlayerHUDWidget.h"

UAmmo::UAmmo()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAmmo::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;

	CurrentAmmoInMag = MagCapacity;

	DisplayAmmo();
}

void UAmmo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAmmo::DisplayAmmo()
{
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->SetAmmoValue(CurrentAmmoInMag, CurrentAmmo);
	}
}

bool UAmmo::EnoughAmmo(int AmmoNeeded)
{
	if (CurrentAmmoInMag >= AmmoNeeded)
	{
		return true;
	}
	MakeReload();
	return false;
}

void UAmmo::UseAmmo(int AmmoUsed)
{
	if (CurrentAmmoInMag <= 0 || IsReloading) return;

	CurrentAmmoInMag -= AmmoUsed;

	DisplayAmmo();
}

void UAmmo::Reload()
{
	if (CurrentAmmoInMag == MagCapacity) return;

	IsReloading = true;
	
	if (CurrentAmmo < MagCapacity)
	{
		CurrentAmmoInMag = CurrentAmmo;
		CurrentAmmo = 0;
	}
	else
	{
		int Difference = MagCapacity - CurrentAmmoInMag;
		CurrentAmmo -= Difference;
		CurrentAmmoInMag += Difference;
	}
	IsReloading = false;
	
	DisplayAmmo();
}

void UAmmo::MakeReload()
{
	if (IsReloading) return;
	
	IsReloading = true;
	ReloadTimerHandler.Invalidate();
	GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimerHandler, this, &UAmmo::Reload, ReloadTime, false);
}

void UAmmo::AddAmmo(int AmmoToAdd)
{
	CurrentAmmo += AmmoToAdd;

	if (CurrentAmmo > MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
	
	DisplayAmmo();
}



