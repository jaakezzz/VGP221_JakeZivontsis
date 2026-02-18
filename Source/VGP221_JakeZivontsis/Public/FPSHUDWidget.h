#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FPSHUDWidget.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API UFPSHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // These functions will be called by the Character when values change
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateHealth(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateAmmo(int32 CurrentAmmo, int32 ReserveAmmo);

    // Function to format and display the time
    void UpdateTimerDisplay(float TimeRemaining);

protected:
    // "BindWidget" tells Unreal: "Look for a component in the Blueprint named EXACTLY this."
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimerText;
};