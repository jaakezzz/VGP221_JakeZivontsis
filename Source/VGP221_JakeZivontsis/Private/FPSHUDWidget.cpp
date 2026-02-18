#include "FPSHUDWidget.h"
#include "Components/TextBlock.h"

void UFPSHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthBar)
    {
        // Safe division check
        float Percent = (MaxHealth > 0) ? (CurrentHealth / MaxHealth) : 0;
        HealthBar->SetPercent(Percent);
    }
}

void UFPSHUDWidget::UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo)
{
    if (AmmoText)
    {
        // FString::Printf is the C++ equivalent of "Format Text"
        FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentAmmo, ReserveAmmo);
        AmmoText->SetText(FText::FromString(AmmoString));
    }
}

void UFPSHUDWidget::UpdateTimerDisplay(float TimeRemaining)
{
    if (!TimerText) return;

    // 1. Calculate Minutes and Seconds
    int32 Minutes = FMath::FloorToInt(TimeRemaining / 60.0f);
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(TimeRemaining, 60.0f));

    // 2. Format the string to always show two digits for seconds (e.g., 05 instead of 5)
    FString TimerString = FString::Printf(TEXT("Timer: %02d:%02d"), Minutes, Seconds);

    // 3. Set the text
    TimerText->SetText(FText::FromString(TimerString));
}