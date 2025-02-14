#include "GameModes/GM_MainMenu.h"
#include "Widgets/MainMenuHUDWidget.h"
#include "Widgets/OptionsHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

void AGM_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		MainMenuWidget = CreateWidget<UMainMenuHUDWidget>(PlayerController, MainMenuWidgetClass);
		OptionsWidget = CreateWidget<UOptionsHUDWidget>(PlayerController, OptionsWidgetClass);
		
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AGM_MainMenu::ShowMainMenu()
{
	// Cache le menu des options
	if (OptionsWidget)
	{
		OptionsWidget->RemoveFromViewport();
	}

	// Affiche le menu principal
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}
}

void AGM_MainMenu::ShowOptionsMenu()
{
	// Cache le menu principal
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromViewport();
	}

	// Affiche le menu des options
	if (OptionsWidget)
	{
		OptionsWidget->AddToViewport();
	}
}
