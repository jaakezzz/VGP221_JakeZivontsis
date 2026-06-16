#include "FPSWinWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameInstance.h"

void UFPSWinWidget::NativeConstruct()
{
    Super::NativeConstruct(); // Always call the parent class first!

    // 1. Get the Game Instance
    if (UFPSGameInstance* GameInst = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        // 2. Format Kills and Collectibles (Standard Integers)
        if (KillsText)
        {
            KillsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInst->RunKills)));
        }

        if (CollectiblesText)
        {
            CollectiblesText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GameInst->RunCollectibles)));
        }

        // 3. Format Time (0:00 format)
        if (TimeText)
        {
            // Convert the total float seconds into a rounded integer
            int32 TotalSeconds = FMath::FloorToInt(GameInst->RunTime);

            // Divide by 60 to get the total minutes
            int32 Minutes = TotalSeconds / 60;

            // Use modulo (%) to get the leftover seconds
            int32 Seconds = TotalSeconds % 60;

            // %d is for minutes, %02d forces the seconds to always have two digits
            FString FormattedTime = FString::Printf(TEXT("%d:%02d"), Minutes, Seconds);
            TimeText->SetText(FText::FromString(FormattedTime));
        }

        // 4. Format Scores (Floats formatted as whole numbers using "%.0f")
        if (ScoreText)
        {
            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), GameInst->CurrentScore)));
        }

        if (HighScoreText)
        {
            HighScoreText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), GameInst->HighScore)));
        }
    }
}
