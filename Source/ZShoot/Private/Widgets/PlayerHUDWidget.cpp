// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CanvasPanel)
	{
		WidgetTree->RootWidget = CanvasPanel;
	}

	HideWaveTimer();
}

void UPlayerHUDWidget::SetHealthValue(float Value)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Value);
		
		FLinearColor HealthColor;
		if (Value <= 0.25f)
		{
			HealthColor = FLinearColor::Red;
		}
		else
		{
			HealthColor = FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Green, Value);
		}
		HealthBar->SetFillColorAndOpacity(HealthColor);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(Value * 100))));
	}
	
}

void UPlayerHUDWidget::SetAmmoValue(int CurrentAmmo, int MaxAmmo)
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
	}
}

void UPlayerHUDWidget::ShowHitMarker()
{
	FLinearColor HitMarkerColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	HitMarker->SetColorAndOpacity(HitMarkerColor);

	GetWorld()->GetTimerManager().SetTimer(HitMarkerTimerHandler, this, &UPlayerHUDWidget::FadeHitMarker, HitMarkerFadeTime, true);
}

void UPlayerHUDWidget::FadeHitMarker()
{
	FLinearColor CurrentColor = HitMarker->GetColorAndOpacity();
	FLinearColor TargetColor = FLinearColor(1.f, 1.f, 1.f, 0.f);

	FLinearColor NewColor = FMath::Lerp(CurrentColor, TargetColor, 0.1f);
	HitMarker->SetColorAndOpacity(NewColor);

	if (NewColor.A <= HitMarkerFadeTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimerHandler);
	}
}

void UPlayerHUDWidget::StartWaveTimer(float Time)
{
	if (WaveTimer)
	{
		WaveTimer->SetText(FText::AsNumber(Time));

		WaveTimer->SetVisibility(ESlateVisibility::Visible);

		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UPlayerHUDWidget::UpdateWaveTimer, 1.0f, true);

		RemainingTime = Time;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveTimer is null!"));
	}
}

void UPlayerHUDWidget::UpdateWaveTimer()
{
	int RoundedTime = FMath::CeilToInt(RemainingTime);
	WaveTimer->SetText(FText::AsNumber(RoundedTime));
	
	if (RemainingTime <= 0)
	{
		WaveTimer->SetText(FText::FromString(TEXT("GO!")));
		
		GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
		
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UPlayerHUDWidget::HideWaveTimer, 1.0f, false);
	}

	RemainingTime -= 1.0f;
}

void UPlayerHUDWidget::HideWaveTimer()
{
	WaveTimer->SetVisibility(ESlateVisibility::Hidden);
}
