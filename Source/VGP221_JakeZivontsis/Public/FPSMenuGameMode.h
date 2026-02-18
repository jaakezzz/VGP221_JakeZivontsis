#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSMenuGameMode.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// 1. The specific widget class to show
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MenuWidgetClass;

	UFUNCTION()
	void HandlePlayButtonClicked();

	UFUNCTION()
	void HandleQuitButtonClicked();

	UFUNCTION()
	void HandleMenuButtonClicked();

protected:
	// 2. Override BeginPlay to run logic when the level starts
	virtual void BeginPlay() override;

	// reference to the widget
	UPROPERTY()
	class UUserWidget* ActiveMenuWidget;
};