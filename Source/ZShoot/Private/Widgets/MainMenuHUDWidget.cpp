#include "Widgets/MainMenuHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/GM_MainMenu.h"
#include "Engine/World.h"

void UMainMenuHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnStartClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnQuitClicked);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuHUDWidget::OnOptionsClicked);
	}
}

void UMainMenuHUDWidget::OnStartClicked()
{
	if (!LevelToLoad.IsNone())
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
		UGameplayStatics::OpenLevel(this, LevelToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelToLoad is not set!"));
	}
}

void UMainMenuHUDWidget::OnQuitClicked()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UMainMenuHUDWidget::OnOptionsClicked()
{
	AGM_MainMenu* MainMenuGameMode = Cast<AGM_MainMenu>(UGameplayStatics::GetGameMode(this));

	if (MainMenuGameMode)
	{
		MainMenuGameMode->ShowOptionsMenu();
	}
}
