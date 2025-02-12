#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSaveGame.generated.h"

/**
 * Classe de sauvegarde pour gérer le High Score
 */
UCLASS()
class ZSHOOT_API UHighScoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UHighScoreSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HighScore")
	int HighScore;

	static int LoadHighScore();
	static void SaveHighScore(int32 NewHighScore);
};
