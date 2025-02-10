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

	if (IsReloading)
	{
		RemainingReloadTime -= DeltaTime;
		if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
		{
			float ReloadProgress = FMath::Clamp((ReloadTime - RemainingReloadTime) / ReloadTime, 0.f, 1.f);
			Player->PlayerHUD->UpdateReloadBar(ReloadProgress);
		}
	}
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

	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->HideReloadBar();
	}
	
	DisplayAmmo();
}

void UAmmo::MakeReload()
{
	if (IsReloading || CurrentAmmoInMag == MagCapacity) return;

	IsReloading = true;
	RemainingReloadTime = ReloadTime;
	ReloadTimerHandler.Invalidate();
	GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimerHandler, this, &UAmmo::Reload, ReloadTime, false);
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->ShowReloadBar();
	}
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



