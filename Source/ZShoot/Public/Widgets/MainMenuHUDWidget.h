#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSHOOT_API UMainMenuHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> StartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	FName LevelToLoad; 

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

	virtual void NativeConstruct() override;
};
