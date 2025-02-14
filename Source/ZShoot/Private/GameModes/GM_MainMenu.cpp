#include "GameModes/GM_MainMenu.h"
#include "Widgets/MainMenuHUDWidget.h"
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

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
}
