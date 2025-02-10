// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCanvasPanel;
class UImage;
/**
 * 
 */
UCLASS()
class ZSHOOT_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowHitMarker();
	void SetHealthValue(float Value);
	void SetAmmoValue(int CurrentAmmo, int MaxAmmo);

	void StartWaveTimer(float Time);

	FTimerHandle WaveTimerHandle;

protected:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly, Category= "Canvas", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, Category= "Health", meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category= "Health", meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(BlueprintReadOnly, Category= "Shoot", meta=(BindWidget))
	TObjectPtr<UImage> HitMarker;

	UPROPERTY(BlueprintReadOnly, Category= "Ammo", meta=(BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(BlueprintReadOnly, Category= "Timer", meta=(BindWidget))
	TObjectPtr<UTextBlock> WaveTimer;
	
	FTimerHandle HitMarkerTimerHandler;
	float HitMarkerFadeTime = 0.05f;
	
private:
	void FadeHitMarker();
	void UpdateWaveTimer();
	void HideWaveTimer();

	float RemainingTime;
};
