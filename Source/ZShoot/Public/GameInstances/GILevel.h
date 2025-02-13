#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GILevel.generated.h"

class UPlayerHUDWidget;
class UGameOverHUD;
class UHighScoreSaveGame;

/**
 * The GameInstance class for managing game level logic, including player HUD, waves, enemies, and high score.
 */
UCLASS()
class ZSHOOT_API UGILevel : public UGameInstance
{
	GENERATED_BODY()

public:
	// Initializes Player HUD and Game Over HUD
	void CreatePlayerHUD();

	// Starts a new wave of enemies
	void StartWave();

	// Restarts the current level
	void RestartLevel() const;

	// Called when an enemy is defeated
	void OnEnemyDeath();

	// Ends the game, handles high score and displays the game over HUD
	UFUNCTION(BlueprintCallable, Category = "HighScore")
	void EndGame();

	// Debug function to simulate a kill count for testing
	UFUNCTION(Exec)
	void Debug_Save(int32 TestKillCount);

protected:
	// Class types for Player HUD and Game Over HUD (can be assigned in the editor)
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	TSubclassOf<UGameOverHUD> GameOverHUDClass;

	// Enemy class used for spawning
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	TSubclassOf<AActor> EnemyClass;

	// Initial number of enemies per wave and the growth rate
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	int InitialEnemy = 5;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float GrowthRate = 0.3f;

	// Enemy spawn time and wave duration
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float SpawnTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float WaveTime = 5.0f;

private:
	// Player HUD and Game Over HUD instances
	UPlayerHUDWidget* PlayerHUD = nullptr;
	UGameOverHUD* GameOverHUD = nullptr;

	// Timer handles for spawning enemies and managing wave timer
	FTimerHandle SpawnTimerHandle;
	FTimerHandle WaveTimerHandle;

	// Wave-related variables
	int EnemiesToSpawn = 0;
	int SpawnedEnemies = 0;
	int CurrentWave = 1;
	int EnemiesAlive = 0;
	int KillCount = 0;

	// Private helper functions
	void SpawnEnemy();
	void NextWave();
	void StartEnemySpawn();
	void LinkHUDToPlayer() const;
	void DisplayGameOverHUD(int Score, int HighScore, int Timer, bool isHighScore);

	// Log the status of the wave (used for debugging purposes)
	static void LogWaveStatus(const FString& Message);
};
