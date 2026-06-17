// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/AudioComponent.h"
#include "FPSGameMode.generated.h"

/**
 *
 */
UCLASS()
class MANNEQUINCHAMBER_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	AFPSGameMode();

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

	// --- AUDIO SYSTEM ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* MusicComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* ChaseStinger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	float StingerCooldown = 0.5f; // Minimum seconds between stinger plays

	// Functions for the enemies to call
	void ReportChaseStarted();
	void ReportChaseEnded();

protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

	// The handle that manages the timer
	FTimerHandle GameTimerHandle;

	// Function called every second to decrement the timer
	void UpdateTimer();

private:
	float LastStingerTime = -100.0f; 
	int32 ActiveChasers = 0; // Master tally of angry enemies

};