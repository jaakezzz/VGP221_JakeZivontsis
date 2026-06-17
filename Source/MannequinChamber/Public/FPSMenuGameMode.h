#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSMenuGameMode.generated.h"

UCLASS()
class MANNEQUINCHAMBER_API AFPSMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSMenuGameMode();

	// 1. The specific widget class to show
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MenuWidgetClass;

	// --- AUDIO ---

	// The looping background track for the menu
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* MenuMusicComponent;

	// The one-shot sound to play the instant the menu opens (Win/Lose stingers)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	class USoundBase* LevelStartStinger;

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