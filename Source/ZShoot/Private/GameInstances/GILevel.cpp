#include "GameInstances/GILevel.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/GameOverHUD.h"
#include "Kismet/GameplayStatics.h"
#include "HighScoreSaveGame.h"
#include "Pawns/PlayerClass.h"

void UGILevel::CreatePlayerHUD()
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0); PlayerController && PlayerHUDClass)
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

void UGILevel::LinkHUDToPlayer() const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APlayerClass* PlayerActor = Cast<APlayerClass>(PlayerController->GetPawn());
	if (PlayerActor && PlayerHUD)
	{
		PlayerActor->PlayerHUD = PlayerHUD;
	}
}

void UGILevel::RestartLevel() const
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void UGILevel::StartWave()
{
    SpawnedEnemies = 0;
    const float RandomFactor = FMath::FRandRange(0.8f, 1.2f);
    EnemiesToSpawn = FMath::CeilToInt(InitialEnemy * FMath::Exp(GrowthRate * (CurrentWave - 1)) * RandomFactor);
    EnemiesAlive = EnemiesToSpawn;

    PlayerHUD ? PlayerHUD->StartWaveTimer(WaveTime) : void();

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

    const FVector SpawnLocation(FMath::RandRange(1000.f, 2000.f), FMath::RandRange(-1500.f, -500.f), 150.f);

    if (AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator))
    {
        SpawnedEnemies++;
    }
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
    PlayerHUD->StopTimer();
    
    // Récupération du high score
    const int HighScore = UHighScoreSaveGame::LoadHighScore();
    const int CurrentTimer = PlayerHUD->GetTimer();
    bool bIsNewHighScore = KillCount > HighScore;

    // Si un nouveau high score est atteint, on l'enregistre
    if (bIsNewHighScore)
    {
        UHighScoreSaveGame::SaveHighScore(KillCount);
        UE_LOG(LogTemp, Log, TEXT("New High Score: %d"), KillCount);
    }

    // Affichage du high score
    UE_LOG(LogTemp, Log, TEXT("Current Kill Count: %d, High Score: %d"), KillCount, bIsNewHighScore ? KillCount : HighScore);

    // Affichage du GameOverHUD
    DisplayGameOverHUD(KillCount, HighScore, CurrentTimer,bIsNewHighScore);
}

void UGILevel::DisplayGameOverHUD(int Score, int HighScore, int Timer, bool isHighScore)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->SetInputMode(FInputModeUIOnly());

        if (GameOverHUDClass)
        {
            GameOverHUD = CreateWidget<UGameOverHUD>(PlayerController, GameOverHUDClass);
            if (GameOverHUD)
            {
                GameOverHUD->UpdateHUD(Score, HighScore, Timer, isHighScore);
                GameOverHUD->AddToViewport();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create GameOverHUD."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GameOverHUDClass is null."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));
    }
}



void UGILevel::Debug_Save(int32 TestKillCount)
{
    KillCount = TestKillCount; // Simulate a score achieved
    EndGame();
}
