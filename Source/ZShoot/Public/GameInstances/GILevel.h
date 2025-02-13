// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GILevel.generated.h"

class UPlayerHUDWidget;
class UHighScoreSaveGame;

/**
 * 
 */
UCLASS()
class ZSHOOT_API UGILevel : public UGameInstance
{
	GENERATED_BODY()

public:
	void CreatePlayerHUD();
	void StartWave();
	void RestartLevel();
	void OnEnemyDeath();

	UFUNCTION(BlueprintCallable, Category = "HighScore")
	void EndGame();

	UFUNCTION(Exec) // Permet d'exécuter la fonction via la console in-game
	void Debug_SaveHighScore(int32 TestKillCount);


protected:
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	int InitialEnemy = 5;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float GrowthRate = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float SpawnTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float WaveTime = 5.0f;

private:
	UPlayerHUDWidget* PlayerHUD = nullptr;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;

	int EnemiesToSpawn = 0;
	int SpawnedEnemies = 0;
	int CurrentWave = 1;
	int EnemiesAlive = 0;
	int KillCount = 0;

	void SpawnEnemy();
	void NextWave();
	void StartEnemySpawn();
	void LinkHUDToPlayer();
	void LogWaveStatus(const FString& Message);
};
