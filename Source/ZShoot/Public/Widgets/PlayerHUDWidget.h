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
	

protected:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly, Category= "Health", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(BlueprintReadOnly, Category= "Health", meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category= "Health", meta=(BindWidget))
	TObjectPtr<UImage> HitMarker;

	FTimerHandle HitMarkerTimerHandler;
	float HitMarkerFadeTime = 0.05f;
	
private:
	void FadeHitMarker();
	
};
