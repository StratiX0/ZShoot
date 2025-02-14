// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OptionsHUDWidget.h"
#include "GameModes/GM_MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UOptionsHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UOptionsHUDWidget::OnReturnClicked);
	}
}

void UOptionsHUDWidget::OnReturnClicked()
{
	AGM_MainMenu* MainMenuGameMode = Cast<AGM_MainMenu>(UGameplayStatics::GetGameMode(this));

	if (MainMenuGameMode)
	{
		MainMenuGameMode->ShowMainMenu();
	}
}
