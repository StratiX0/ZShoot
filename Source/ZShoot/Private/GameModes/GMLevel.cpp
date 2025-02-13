// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GMLevel.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstances/GILevel.h"

void AGMLevel::BeginPlay()
{
	Super::BeginPlay();
	InitializeGame();

	int HighScore = LoadHighScore();
	UE_LOG(LogTemp, Log, TEXT("High Score loaded: %d"), HighScore);
}

void AGMLevel::SaveHighScore(int NewHighScore)
{
	// Créer une instance de la classe SaveGame
	UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));

	// Assigner le high score
	SaveGameInstance->HighScore = NewHighScore;

	// Sauvegarder les données dans un fichier (dans un slot "HighScoreSlot")
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("HighScoreSlot"), 0);
	UE_LOG(LogTemp, Log, TEXT("High Score saved: %d"), NewHighScore);
}

int32 AGMLevel::LoadHighScore()
{
	// Charger le jeu depuis le slot de sauvegarde
	UHighScoreSaveGame* LoadedGame = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScoreSlot"), 0));

	// Si le fichier de sauvegarde existe, retourne le high score
	if (LoadedGame)
	{
		return LoadedGame->HighScore;
	}
	else
	{
		// Si aucune sauvegarde n'est trouvée, retourner 0 comme valeur par défaut
		return 0;
	}
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
