#include "GameInstances/GILevel.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HighScoreSaveGame.h"
#include "Pawns/PlayerClass.h"

void UGILevel::CreatePlayerHUD()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
			LinkHUDToPlayer();
			PlayerHUD->StartTimer();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create PlayerHUD."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController or PlayerHUDClass is null."));
	}
}

void UGILevel::LinkHUDToPlayer()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APlayerClass* PlayerActor = Cast<APlayerClass>(PlayerController->GetPawn());
	if (PlayerActor && PlayerHUD)
	{
		PlayerActor->PlayerHUD = PlayerHUD;
	}
}

void UGILevel::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void UGILevel::StartWave()
{
	SpawnedEnemies = 0;
	float RandomFactor = FMath::FRandRange(0.8f, 1.2f);
	EnemiesToSpawn = FMath::CeilToInt(InitialEnemy * FMath::Exp(GrowthRate * (CurrentWave - 1)) * RandomFactor);
	EnemiesAlive = EnemiesToSpawn;

	if (PlayerHUD)
	{
		PlayerHUD->StartWaveTimer(WaveTime);
	}

	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UGILevel::StartEnemySpawn, WaveTime, false);
	LogWaveStatus(FString::Printf(TEXT("Wave %d started with %d enemies."), CurrentWave, EnemiesToSpawn));
}

void UGILevel::StartEnemySpawn()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UGILevel::SpawnEnemy, SpawnTime, true);
}

void UGILevel::SpawnEnemy()
{
	if (SpawnedEnemies >= EnemiesToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		LogWaveStatus(TEXT("All enemies spawned for current wave."));
		return;
	}

	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyClass is null."));
		return;
	}

	FVector SpawnLocation(FMath::RandRange(1000.f, 2000.f), FMath::RandRange(-1500.f, -500.f), 150.f);
	GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	SpawnedEnemies++;
}

void UGILevel::OnEnemyDeath()
{
	EnemiesAlive--;
	UE_LOG(LogTemp, Warning, TEXT("Enemy defeated! Remaining: %d."), EnemiesAlive);

	if (PlayerHUD)
	{
		KillCount++;
		PlayerHUD->IncreaseKillCount(1);
	}

	if (EnemiesAlive <= 0)
	{
		NextWave();
	}
}

void UGILevel::NextWave()
{
	CurrentWave++;
	StartWave();
	LogWaveStatus(FString::Printf(TEXT("Wave %d completed. Preparing next wave..."), CurrentWave - 1));
}

void UGILevel::LogWaveStatus(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

void UGILevel::EndGame()
{
	UE_LOG(LogTemp, Log, TEXT("Current Kill Count: %d"), KillCount);

	int HighScore = UHighScoreSaveGame::LoadHighScore();

	if (KillCount > HighScore)
	{
		UHighScoreSaveGame::SaveHighScore(KillCount);
		UE_LOG(LogTemp, Log, TEXT("New High Score: %d"), KillCount);
		HighScore = KillCount;
	}

	UE_LOG(LogTemp, Log, TEXT("High Score: %d"), HighScore);
}

void UGILevel::Debug_SaveHighScore(int32 TestKillCount)
{
	KillCount = TestKillCount;  // Simule un score atteint

	EndGame();
}