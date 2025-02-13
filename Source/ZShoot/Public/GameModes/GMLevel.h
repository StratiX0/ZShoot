// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HighScoreSaveGame.h"
#include "GMLevel.generated.h"

/**
 * 
 */
UCLASS()
class ZSHOOT_API AGMLevel : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "HighScore")
	void SaveHighScore(int NewHighScore);

	// Fonction pour charger le high score
	UFUNCTION(BlueprintCallable, Category = "HighScore")
	int LoadHighScore();

private:
	void InitializeGame();
};
