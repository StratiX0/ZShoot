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

	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->SetAmmoValue(CurrentAmmo, MaxAmmo);
	}
}

void UAmmo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAmmo::UseAmmo(int AmmoUsed)
{
	CurrentAmmo -= AmmoUsed;
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->SetAmmoValue(CurrentAmmo, MaxAmmo);
	}
}

void UAmmo::AddAmmo(int AmmoToAdd)
{
	CurrentAmmo += AmmoToAdd;

	if (CurrentAmmo > MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
	
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->SetAmmoValue(CurrentAmmo, MaxAmmo);
	}
}



