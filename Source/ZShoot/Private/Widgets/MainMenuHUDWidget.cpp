#include "Widgets/MainMenuHUDWidget.h"
#include "Kismet/GameplayStatics.h"
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
