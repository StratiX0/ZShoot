// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CanvasPanel)
	{
		WidgetTree->RootWidget = CanvasPanel;

		if (HealthBar)
		{
			HealthBar->SetPercent(75.f);
		}
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
