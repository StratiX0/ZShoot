#include "GameInstances/GILevel.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerActor.h"

void UGILevel::CreatePlayerHUD()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerHUD is nullptr after creation."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController or PlayerHUDClass is nullptr."));
	}

	APlayerActor* PlayerActor = Cast<APlayerActor>(PlayerController->GetPawn());

	if (PlayerActor)
	{
		PlayerActor->PlayerHUD = Cast<UPlayerHUDWidget>(PlayerHUD);
	}
}

void UGILevel::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void UGILevel::StartWave()
{
	SpawnedEnemies = 0;
	EnemiesAlive = EnemiesPerWave * CurrentWave;
    
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UGILevel::SpawnEnemy, 2.0f, true);
}

void UGILevel::SpawnEnemy()
{
	if (SpawnedEnemies < EnemiesAlive)
	{
		if (!EnemyClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("EnemyClass est nul."));
			return;
		}

		float RandomX = FMath::RandRange(1000.f, 2000.f);
		float RandomY = FMath::RandRange(-1500.f, -500.f);

		FVector SpawnLocation(RandomX, RandomY, 60.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);
		SpawnedEnemies++;

		UE_LOG(LogTemp, Warning, TEXT("Vague %d lancée avec %d ennemis."), CurrentWave, EnemiesAlive);
	}
	else
	{
		// Arrêter le timer une fois que tous les ennemis ont été spawners
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Tous les ennemis ont été spawnés pour la vague %d."), CurrentWave);
	}
}

void UGILevel::OnEnemyDeath()
{
	EnemiesAlive--;
	UE_LOG(LogTemp, Warning, TEXT("Ennemi éliminé!"));

	if (EnemiesAlive <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vague %d terminée. Préparation de la suivante..."), CurrentWave);

		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UGILevel::NextWave, 5.0f, false);
	}
}

void UGILevel::NextWave()
{
	CurrentWave++;
	UE_LOG(LogTemp, Warning, TEXT("Vague %d terminée. Préparation de la suivante..."), CurrentWave - 1);
	StartWave();
}