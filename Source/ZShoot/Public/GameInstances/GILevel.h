// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GILevel.generated.h"

class UPlayerHUDWidget;
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

	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;

protected:

private:
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	UPlayerHUDWidget* PlayerHUD;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	TSubclassOf<AActor> EnemyClass;
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	int EnemiesPerWave = 2;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float SpawnTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float WaveTime = 5.0f;

	
	int SpawnedEnemies = 0;
	int CurrentWave = 1;
	int EnemiesAlive = 0;
	void SpawnEnemy();
	void NextWave();
	void StartEnemySpawn();
};
