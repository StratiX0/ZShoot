#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCanvasPanel;
class UImage;

UCLASS()
class ZSHOOT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// Core Functions
	void ShowHitMarker();
	void SetHealthValue(float Value);
	void SetAmmoValue(int CurrentAmmo, int MaxAmmo);
	void UpdateReloadBar(float Value);
	void ShowReloadBar();
	void HideReloadBar();
	void StartWaveTimer(float Time);
	void StartTimer();
	void StopTimer();
	int GetTimer() const;
	void IncreaseKillCount(int Kill);
	void SetWave(int WaveNumber);
	void UpdateBloodSplash(float DeltaTime);
	void SetBloodSplashVisibility(bool bVisible) { bUpdateBloodSplash = bVisible; }

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Category = "Canvas", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UImage> HitMarker;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UImage> BloodSplashIMG;

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText1;

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText2;

	UPROPERTY(BlueprintReadOnly, Category = "Reload", meta = (BindWidget))
	TObjectPtr<UProgressBar> ReloadBar;

	UPROPERTY(BlueprintReadOnly, Category = "Timer", meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Timer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(BlueprintReadOnly, Category = "Kills", meta = (BindWidget))
	TObjectPtr<UTextBlock> KillsNumberText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;

private:
	// Timers
	FTimerHandle HitMarkerTimerHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle TimerHandle;

	// Properties
	float HitMarkerFadeTime = 0.05f;
	float HitMarkerFadeSpeed = 10.f;
	float RemainingTime;
	int KillCount = 0;
	int GameTimer = 0;
	int Wave = 0;
	bool bUpdateBloodSplash = false;

	UPROPERTY(EditAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	float BloodSplashSpeed = 1.0f;

	// Helper Functions
	void FadeHitMarker();
	void UpdateWaveTimer();
	void HideWaveTimer();
	void UpdateTimer();
	static void SetTextSafe(UTextBlock* TextBlock, const FString& Text);
	static void ToggleVisibility(UWidget* Widget, bool bVisible);
};
