// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GMLevel.h"
#include "GameInstances/GILevel.h"

void AGMLevel::BeginPlay()
{
	Super::BeginPlay();

	UGILevel* GameInstance = Cast<UGILevel>(GetGameInstance());
	GameInstance->CreatePlayerHUD();
}
