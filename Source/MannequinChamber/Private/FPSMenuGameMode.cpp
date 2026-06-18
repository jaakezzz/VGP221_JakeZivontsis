// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSMenuGameMode.h"
#include "Blueprint/UserWidget.h" // Needed to create widgets
#include "Kismet/KismetSystemLibrary.h" // Needed for the Quit function
#include "Kismet/GameplayStatics.h" // Needed for OpenLevel
#include "Components/Button.h"         // Needed to talk to the Button class
#include "Components/AudioComponent.h"
#include "FPSGameInstance.h" // Needed to access the GameInstance for level references


AFPSMenuGameMode::AFPSMenuGameMode()
{
	// Create the background music component
	MenuMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MenuMusicComponent"));

	// Ensure it plays (2D)
	MenuMusicComponent->bAllowSpatialization = false;
}

void AFPSMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// --- AUDIO ---
	// If a one-shot stinger was assigned in the Blueprint, play it immediately
	if (LevelStartStinger)
	{
		UGameplayStatics::PlaySound2D(this, LevelStartStinger);
	}

	// 1. Check if the blueprint has been assigned
	if (MenuWidgetClass)
	{
		// Create the widget
		ActiveMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);

		if (ActiveMenuWidget)
		{
			// Add to screen
			ActiveMenuWidget->AddToViewport();

			// -- BIND QUIT BUTTON --
			if (UButton* QuitBtn = Cast<UButton>(ActiveMenuWidget->GetWidgetFromName(TEXT("Btn_Quit"))))
			{
				// Link the Button's OnClicked event to our C++ function
				QuitBtn->OnClicked.AddDynamic(this, &AFPSMenuGameMode::HandleQuitButtonClicked);
			}

			// --- BIND PLAY BUTTON ---
			if (UButton* PlayBtn = Cast<UButton>(ActiveMenuWidget->GetWidgetFromName(TEXT("Btn_Play"))))
			{
				PlayBtn->OnClicked.AddDynamic(this, &AFPSMenuGameMode::HandlePlayButtonClicked);
			}

			// --- BIND MENU BUTTON ---
			if (UButton* MenuBtn = Cast<UButton>(ActiveMenuWidget->GetWidgetFromName(TEXT("Btn_Menu"))))
			{
				MenuBtn->OnClicked.AddDynamic(this, &AFPSMenuGameMode::HandleMenuButtonClicked);
			}
		}
	}

	// 2. Setup the Input Mode (Show Mouse, Freeze Player)
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// Show the cursor
		PC->bShowMouseCursor = true;

		// Tell the engine: "Only let the player click on UI, don't let them look around"
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(nullptr);
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputModeData);
	}
}

void AFPSMenuGameMode::HandlePlayButtonClicked()
{
	// Ask the GameInstance for the Gameplay Map
	if (UFPSGameInstance* GameInst = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (!GameInst->GameplayLevel.IsNull())
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameInst->GameplayLevel);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Gameplay Level is not assigned in the GameInstance Blueprint"));
		}
	}
}

void AFPSMenuGameMode::HandleMenuButtonClicked()
{
	// Ask the GameInstance for the Main Menu Map
	if (UFPSGameInstance* GameInst = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (!GameInst->MainMenuLevel.IsNull())
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameInst->MainMenuLevel);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Main Menu is not assigned in the GameInstance Blueprint"));
		}
	}
}

void AFPSMenuGameMode::HandleQuitButtonClicked()
{
	// UKismetSystemLibrary::QuitGame is the "cleanest" way to exit in UE5
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

