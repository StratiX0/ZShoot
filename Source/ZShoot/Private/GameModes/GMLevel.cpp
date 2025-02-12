// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GMLevel.h"
#include "GameInstances/GILevel.h"

void AGMLevel::BeginPlay()
{
	Super::BeginPlay();
	InitializeGame();
}

void AGMLevel::InitializeGame()
{
	UGILevel* GameInstance = Cast<UGILevel>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->CreatePlayerHUD();
		GameInstance->StartWave();

		UE_LOG(LogTemp, Log, TEXT("Game initialized successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UGILevel. Check GameInstance settings."));
	}
}
