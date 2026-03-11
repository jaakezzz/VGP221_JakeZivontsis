#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSWinWidget.generated.h" 

UCLASS()
class VGP221_JAKEZIVONTSIS_API UFPSWinWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    // This is the C++ version of "Event Construct"
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KillsText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CollectiblesText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HighScoreText;
};