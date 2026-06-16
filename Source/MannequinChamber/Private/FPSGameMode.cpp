// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "FPSHUDWidget.h"
#include "Kismet/GameplayStatics.h"
// #include "TimerManager.h"

void AFPSGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameModeBase! This is hybrid Build"));
}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        // 1. Hide the mouse cursor for gameplay
        PC->bShowMouseCursor = false;

        // 2. Tell the engine to ignore UI and focus on the game world
        FInputModeGameOnly InputModeData;

        // This ensures the mouse stays inside the game window
        InputModeData.SetConsumeCaptureMouseDown(true);

        PC->SetInputMode(InputModeData);

        // Initialize the countdown
        TimeRemaining = LevelTimeLimit;

        // Start the timer to call UpdateTimer every 1.0 seconds, looping
        GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AFPSGameMode::UpdateTimer, 1.0f, true);
    }
}

void AFPSGameMode::UpdateTimer()
{
    if (TimeRemaining > 0)
    {
        TimeRemaining -= 1.0f;

        // --- Update the HUD ---
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            // Get the character, then get the HUD reference stored there
            if (AFPSCharacter* MyChar = Cast<AFPSCharacter>(PC->GetPawn()))
            {
                if (MyChar->MyHUD)
                {
                    MyChar->MyHUD->UpdateTimerDisplay(TimeRemaining);
                }
            }
        }
    }
    else
    {
        // Stop the timer from running further
        GetWorldTimerManager().ClearTimer(GameTimerHandle);

        // Load the Lose Menu
        UGameplayStatics::OpenLevel(GetWorld(), FName("LoseMenu"));
    }
}