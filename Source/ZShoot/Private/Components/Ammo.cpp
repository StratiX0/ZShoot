// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Ammo.h"

UAmmo::UAmmo()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAmmo::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
}

void UAmmo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



