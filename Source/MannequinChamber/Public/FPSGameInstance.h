#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPSGameInstance.generated.h"

UCLASS()
class MANNEQUINCHAMBER_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// This variable will persist as long as the game is open
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float HighScore = 0.0f;

	// Keeps track of the score for the current run
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float CurrentScore = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	int32 RunKills = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	int32 RunCollectibles = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float RunTime = 0.0f;

	// --- GLOBAL MAP REFERENCES (Single Source of Truth) ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Transitions")
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Transitions")
	TSoftObjectPtr<UWorld> GameplayLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Transitions")
	TSoftObjectPtr<UWorld> WinMenuLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Transitions")
	TSoftObjectPtr<UWorld> LoseMenuLevel;
};