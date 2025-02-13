#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameOverHUD.generated.h"

/**
 * 
 */
UCLASS()
class ZSHOOT_API UGameOverHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHUD(int Score, int HighScore, float Timer, bool bIsNewHighScore);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Stats")
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Stats")
	TObjectPtr<UTextBlock> HighScoreText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Stats")
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName MainMenuLevelName = FName("MainMenu");

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Button")
	TObjectPtr<UButton> HomeButton;

	UFUNCTION()
	void RestartLevel();

	UFUNCTION()
	void GoToMainMenu();

	// Called when the widget is constructed
	virtual void NativeConstruct() override;

private:
	int FinalScore;
	int FinalHighScore;
	float FinalTimer;
	bool bIsNewHighScore;
};
