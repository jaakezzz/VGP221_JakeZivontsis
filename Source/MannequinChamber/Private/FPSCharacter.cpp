#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "FPSHUDWidget.h"
#include "Blueprint/UserWidget.h"  // <--- Allows to use CreateWidget()
#include "Kismet/GameplayStatics.h" // <--- Allows to use OpenLevel() // no longer used
#include "FPSGameInstance.h" // <--- Allows access to the GameInstance for level references

AFPSCharacter::AFPSCharacter()
{
    // Create Camera
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0, 0, 60.f)); // Eye height
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create Gun Mesh
    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(FirstPersonCameraComponent);
    GunMesh->SetCastShadow(false);
	// Position Gun Mesh in the Blueprint

    // Create Flashlight
    FlashlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
    FlashlightComp->SetupAttachment(FirstPersonCameraComponent);
    FlashlightComp->SetRelativeLocation(FVector(0, 0, 0)); // Sit right on the camera
    FlashlightComp->SetIntensity(5000.0f); // Brightness
    FlashlightComp->SetOuterConeAngle(25.0f); // tight beam
    FlashlightComp->SetVisibility(false); // Start turned off
}

void AFPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        // Add Input Mapping Context
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }

        // Create the widget and save the reference
        // Create a TSubclassOf<UUserWidget> property to assign WBP_GameHUD in the editor
        if (HUDWidgetClass)
        {
            MyHUD = CreateWidget<UFPSHUDWidget>(PlayerController, HUDWidgetClass);
            if (MyHUD)
            {
                MyHUD->AddToViewport();

				// Set initial values on the HUD
                MyHUD->UpdateHealth(Health, MaxHealth);
                MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
                MyHUD->UpdateCollectibles(Collectibles);
            }
        }
    }

    // Initialize Stats
    Health = MaxHealth;
    CurrentClipAmmo = 0;
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Move);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AFPSCharacter::Fire);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSCharacter::Look);
        EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AFPSCharacter::ToggleFlashlight);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AFPSCharacter::Reload);
    }
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AFPSCharacter::Fire()
{
    if (ProjectileClass && GetWorld())
    {
        // 1. Check Ammo
        if (!CanFire())
        {
            Reload(); // Auto-reload for convenience
            return;
        }

        // 2. Play Gunshot Sound
        if (GunShotSound)
        {
            UGameplayStatics::PlaySound2D(this, GunShotSound);
        }

        // 3. Decrement Ammo
        CurrentClipAmmo--;

        if (MyHUD)
        {
            MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
        }

        FVector MuzzleLocation = GunMesh->GetSocketLocation(TEXT("Muzzle"));
        // If the mesh has no socket named Muzzle, it defaults to the mesh root
        if (GunMesh->DoesSocketExist(TEXT("Muzzle")) == false)
        {
            MuzzleLocation = GunMesh->GetComponentLocation() + (GetActorForwardVector() * 50.0f);
        }

        FRotator MuzzleRotation = GetControlRotation();

        GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation);
    }
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Add Yaw (Left/Right) and Pitch (Up/Down) input
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AFPSCharacter::ToggleFlashlight()
{
    if (FlashlightComp)
    {
        if (FlashlightClickSound)
        {
            UGameplayStatics::PlaySound2D(this, FlashlightClickSound);
        }

        FlashlightComp->ToggleVisibility();
    }
}

void AFPSCharacter::TakeDamageVS(float DamageAmount)
{
    // Play the hurt sound // Audio Cooldown Check
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (PlayerHurtSound && (CurrentTime - LastHurtSoundTime >= HurtSoundCooldown))
    {
        UGameplayStatics::PlaySound2D(this, PlayerHurtSound);
        LastHurtSoundTime = CurrentTime; // Reset the clock
    }

    Health -= DamageAmount;

    if (Health <= 0.0f)
    {
        Health = 0.0f;
        
        // Ask the GameInstance for the Lose Map
        if (UFPSGameInstance* GameInst = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
        {
            if (!GameInst->LoseMenuLevel.IsNull())
            {
                UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameInst->LoseMenuLevel);
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("DEAD! Health: 0"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Health: %f"), Health);
    }

    if (MyHUD)
    {
        MyHUD->UpdateHealth(Health, MaxHealth);
    }
}

void AFPSCharacter::Heal(float HealAmount)
{
    // Play the heal sound // Audio Cooldown Check
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (PlayerHealSound && (CurrentTime - LastHealSoundTime >= HealSoundCooldown))
    {
        UGameplayStatics::PlaySound2D(this, PlayerHealSound);
        LastHealSoundTime = CurrentTime;
    }

    Health += HealAmount;

    // Clamp so we don't go over 100
    if (Health > MaxHealth)
    {
        Health = MaxHealth;
    }
    UE_LOG(LogTemp, Log, TEXT("Healed! Health: %f"), Health);

    if (MyHUD)
    {
        MyHUD->UpdateHealth(Health, MaxHealth);
    }
}

void AFPSCharacter::Reload()
{
    // 1. Safety Check: Don't reload if we are already reloading, clip is full, or reserve is empty
    if (bIsReloading || CurrentClipAmmo == MaxClipSize || MaxReserveAmmo <= 0)
    {
        return;
    }

    // 2. Lock the gun
    bIsReloading = true;

    // 3. Play the sound right as the reload starts
    if (ReloadSound)
    {
        UGameplayStatics::PlaySound2D(this, ReloadSound);
    }

    // 4. Start the Timer! Call FinishReload() after 'ReloadTime' seconds have passed.
    GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFPSCharacter::FinishReload, ReloadTime, false);

    UE_LOG(LogTemp, Log, TEXT("Reloading..."));
}

void AFPSCharacter::FinishReload()
{
    // 1. Unlock the gun
    bIsReloading = false;

    // 2. Do the Ammo Math
    int32 AmmoNeeded = MaxClipSize - CurrentClipAmmo;

    if (MaxReserveAmmo >= AmmoNeeded)
    {
        MaxReserveAmmo -= AmmoNeeded;
        CurrentClipAmmo = MaxClipSize;
    }
    else
    {
        CurrentClipAmmo += MaxReserveAmmo;
        MaxReserveAmmo = 0;
    }

    UE_LOG(LogTemp, Log, TEXT("Reload Finished! Clip: %d | Reserve: %d"), CurrentClipAmmo, MaxReserveAmmo);

    // 3. Update the UI
    if (MyHUD)
    {
        MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
    }
}

bool AFPSCharacter::CanFire() const
{
    return CurrentClipAmmo > 0 && !bIsReloading;
}

void AFPSCharacter::AddReserveAmmo(int32 AmmoAmount)
{
    MaxReserveAmmo += AmmoAmount;

    // Refresh the HUD so the player sees the new ammo
    if (MyHUD)
    {
        MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
    }
    UE_LOG(LogTemp, Log, TEXT("Picked up ammo! Total Reserve: %d"), MaxReserveAmmo);
}

void AFPSCharacter::AddCollectible()
{
    Collectibles++;
    UE_LOG(LogTemp, Log, TEXT("Collectibles Found: %d"), Collectibles);

    if (MyHUD)
    {
        MyHUD->UpdateCollectibles(Collectibles);
    }
}

void AFPSCharacter::AddEnemyKill()
{
    EnemiesKilled++;
    UE_LOG(LogTemp, Log, TEXT("Enemy Killed! Total: %d"), EnemiesKilled);
}

void AFPSCharacter::StartFlashlightGlitch()
{
    // Prevent the trigger box from starting multiple overlapping loops if they touch it twice
    if (bIsFlashlightGlitching) return;

    bIsFlashlightGlitching = true;

    // Start the unpredictable loop
    QueueNextFlicker();
}

void AFPSCharacter::QueueNextFlicker()
{
    // Pick a random time between 3 and 10 seconds
    float RandomDelay = FMath::RandRange(3.0f, 10.0f);

    // Start the timer to execute the first ghost button press
    GetWorldTimerManager().SetTimer(FlickerTimerHandle, this, &AFPSCharacter::FirstFlicker, RandomDelay, false);
}

void AFPSCharacter::FirstFlicker()
{
    // First ghost press
    ToggleFlashlight();

    // Wait exactly 0.15 seconds, then press again
    GetWorldTimerManager().SetTimer(FlickerTimerHandle, this, &AFPSCharacter::SecondFlicker, 0.15f, false);
}

void AFPSCharacter::SecondFlicker()
{
    // Second ghost press
    ToggleFlashlight();

    // Restart the random waiting period
    QueueNextFlicker();
}