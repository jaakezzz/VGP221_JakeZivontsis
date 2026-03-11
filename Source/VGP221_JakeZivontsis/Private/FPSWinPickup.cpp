#include "FPSWinPickup.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "FPSGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AFPSWinPickup::OnPickup(AFPSCharacter* Player)
{
    if (Player)
    {
        // 1. Get references to the Game Mode and Game Instance
        AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        UFPSGameInstance* GameInst = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

        if (GameMode && GameInst)
        {
            // 2. Calculate the time it took to win
            float TimeTaken = GameMode->LevelTimeLimit - GameMode->TimeRemaining;

            // Safety check to prevent dividing by zero!
            if (TimeTaken <= 0.0f) TimeTaken = 1.0f;

            // 3. THE FORMULA: ((Collectibles + Kills) / TimeTaken) * 18000
            float TotalItemsAndKills = Player->Collectibles + Player->EnemiesKilled;
            float FinalScore = (TotalItemsAndKills / TimeTaken) * 18000.0f;

            // 4. Save to Game Instance
            GameInst->CurrentScore = FinalScore;

            // Save the context for the UI
            GameInst->RunKills = Player->EnemiesKilled;
            GameInst->RunCollectibles = Player->Collectibles;
            GameInst->RunTime = TimeTaken;

            if (FinalScore > GameInst->HighScore)
            {
                GameInst->HighScore = FinalScore; // New High Score!
            }

            UE_LOG(LogTemp, Warning, TEXT("YOU WIN! Final Score: %f"), FinalScore);
        }

        // 5. Load the Win Menu
        UGameplayStatics::OpenLevel(GetWorld(), FName("WinMenu"));
        Destroy();
    }
}