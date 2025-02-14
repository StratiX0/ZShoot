#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GM_MainMenu.generated.h"

class UMainMenuHUDWidget;
class UOptionsHUDWidget;

UCLASS()
class ZSHOOT_API AGM_MainMenu : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainMenuHUDWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UOptionsHUDWidget> OptionsWidgetClass;
	
	UMainMenuHUDWidget* MainMenuWidget;
	UOptionsHUDWidget* OptionsWidget;

	void ShowMainMenu();
	void ShowOptionsMenu();

	virtual void BeginPlay() override;
};
