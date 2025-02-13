#include "Widgets/PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleVisibility(WaveTimer, false);
	ToggleVisibility(ReloadBar, false);
}

void UPlayerHUDWidget::SetHealthValue(float Value)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Value);
		FLinearColor HealthColor = (Value <= 0.25f) 
			? FLinearColor::Red 
			: FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Green, Value);
		HealthBar->SetFillColorAndOpacity(HealthColor);
	}
	SetTextSafe(HealthText, FString::Printf(TEXT("%d"), FMath::RoundToInt(Value * 100)));
}

void UPlayerHUDWidget::SetAmmoValue(int CurrentAmmo, int MaxAmmo)
{
	SetTextSafe(AmmoText1, FString::Printf(TEXT("%d"), CurrentAmmo));
	SetTextSafe(AmmoText2, FString::Printf(TEXT("%d"), MaxAmmo));
}

void UPlayerHUDWidget::UpdateReloadBar(float Value)
{
	if (ReloadBar) ReloadBar->SetPercent(Value);
}

void UPlayerHUDWidget::ShowReloadBar() { ToggleVisibility(ReloadBar, true); }
void UPlayerHUDWidget::HideReloadBar() { ToggleVisibility(ReloadBar, false); }

void UPlayerHUDWidget::ShowHitMarker()
{
	if (HitMarker)
	{
		HitMarker->SetColorAndOpacity(FLinearColor::White);
		GetWorld()->GetTimerManager().SetTimer(HitMarkerTimerHandle, this, &UPlayerHUDWidget::FadeHitMarker, HitMarkerFadeTime, true);
	}
}

void UPlayerHUDWidget::FadeHitMarker()
{
	if (!HitMarker) return;

	FLinearColor CurrentColor = HitMarker->GetColorAndOpacity();
	CurrentColor.A = FMath::FInterpTo(CurrentColor.A, 0.f, GetWorld()->GetDeltaSeconds(), HitMarkerFadeSpeed);
	HitMarker->SetColorAndOpacity(CurrentColor);

	if (CurrentColor.A <= 0.01f)
	{
		GetWorld()->GetTimerManager().ClearTimer(HitMarkerTimerHandle);
		HitMarker->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
}

void UPlayerHUDWidget::StartWaveTimer(float Time)
{
	RemainingTime = Time;
	SetTextSafe(WaveTimer, TEXT("Wave Start!"));
	ToggleVisibility(WaveTimer, true);

	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UPlayerHUDWidget::UpdateWaveTimer, 1.0f, true);
}

void UPlayerHUDWidget::UpdateWaveTimer()
{
	if (RemainingTime <= 0)
	{
		SetTextSafe(WaveTimer, TEXT("GO!"));
		GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &UPlayerHUDWidget::HideWaveTimer, 1.0f, false);
	}
	else
	{
		SetTextSafe(WaveTimer, FString::FromInt(FMath::CeilToInt(RemainingTime)));
		RemainingTime -= 1.0f;
	}
}

void UPlayerHUDWidget::HideWaveTimer() { ToggleVisibility(WaveTimer, false); }

void UPlayerHUDWidget::IncreaseKillCount(int Kill)
{
	KillCount += Kill;
	SetTextSafe(KillsNumberText, FString::Printf(TEXT("%d"), KillCount));
}

void UPlayerHUDWidget::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerHUDWidget::UpdateTimer, 1.0f, true);
}

<<<<<<< Updated upstream
=======
void UPlayerHUDWidget::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


int UPlayerHUDWidget::GetTimer() const
{
	return GameTimer;
}

>>>>>>> Stashed changes
void UPlayerHUDWidget::UpdateTimer()
{
	GameTimer++;
	SetTextSafe(TimerText, FString::Printf(TEXT("%d'%02d"), GameTimer / 60, GameTimer % 60));
}

// --- Helper Functions ---

void UPlayerHUDWidget::SetTextSafe(UTextBlock* TextBlock, const FString& Text)
{
	if (TextBlock) TextBlock->SetText(FText::FromString(Text));
}

void UPlayerHUDWidget::ToggleVisibility(UWidget* Widget, bool bVisible)
{
	if (Widget)
		Widget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
