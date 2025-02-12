#include "HighScoreSaveGame.h"
#include "Kismet/GameplayStatics.h"

UHighScoreSaveGame::UHighScoreSaveGame()
{
	HighScore = 0;  // Valeur par défaut
}

// Charger le High Score depuis le slot de sauvegarde
int UHighScoreSaveGame::LoadHighScore()
{
	UHighScoreSaveGame* LoadedGame = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScoreSlot"), 0));

	if (LoadedGame)
	{
		UE_LOG(LogTemp, Log, TEXT("High Score loaded: %d"), LoadedGame->HighScore);
		return LoadedGame->HighScore;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No existing High Score found. Returning 0."));
		return 0;
	}
}

void UHighScoreSaveGame::SaveHighScore(int32 NewHighScore)
{
	UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->HighScore = NewHighScore;

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("HighScoreSlot"), 0))
		{
			UE_LOG(LogTemp, Log, TEXT("High Score saved successfully: %d"), NewHighScore);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save High Score."));
		}
	}
}
