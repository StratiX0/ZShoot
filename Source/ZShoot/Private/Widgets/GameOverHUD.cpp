#include "Widgets/GameOverHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverHUD::UpdateHUD(int Score, int HighScore, float Timer, bool bNewHighScore)
{
	// Use the new local variable name
	if (ScoreText)
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
	}

	if (HighScoreText)
	{
		if (bNewHighScore) // Use the renamed parameter
		{
			HighScoreText->SetText(FText::FromString(TEXT("New")));
		}
		else
		{
			HighScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HighScore)));
		}
	}

	if (TimeText)
	{
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Timer)));
	}
}


void UGameOverHUD::RestartLevel()
{
	if (RestartButton)
	{
		RestartButton->SetIsEnabled(false);
	}
	UWorld* World = GetWorld();
	if (World)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}

		// Restart the current level
		UGameplayStatics::OpenLevel(World, FName(*World->GetMapName())); 
	}
}

void UGameOverHUD::GoToMainMenu()
{
	if (HomeButton)
	{
		HomeButton->SetIsEnabled(false);
	}
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, MainMenuLevelName);
	}
}



// Implémentation de la méthode qui lie les boutons
void UGameOverHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameOverHUD::RestartLevel);
	}

	if (HomeButton)
	{
		HomeButton->OnClicked.AddDynamic(this, &UGameOverHUD::GoToMainMenu);
	}
}
