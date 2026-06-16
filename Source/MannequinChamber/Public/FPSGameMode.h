// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

/**
 *
 */
UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()


	virtual void StartPlay() override;

public:
	// Total time allowed for the level (30 minutes = 1800 seconds)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Rules")
	float LevelTimeLimit = 1800.0f;

	// hold the high score across runs
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float HighScore = 0.0f;

	// hold the score of the current run
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float CurrentScore = 0.0f;

	// The actual countdown variable
	float TimeRemaining;

protected:
	virtual void BeginPlay() override;

	// The handle that manages the timer
	FTimerHandle GameTimerHandle;

	// Function called every second to decrement the timer
	void UpdateTimer();

};