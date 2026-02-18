#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPSGameInstance.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// This variable will persist as long as the game is open
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	float HighScore = 0.0f;
};