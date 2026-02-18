#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "FPSHUDWidget.h"
#include "Blueprint/UserWidget.h"  // <--- Allows to use CreateWidget()
#include "Kismet/GameplayStatics.h" // <--- Allows to use OpenLevel()

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
                MyHUD->UpdateHealth(Health, MaxHealth); // Set initial state
                MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
            }
        }
    }

    // Initialize Stats
    Health = MaxHealth;
    CurrentClipAmmo = MaxClipSize;
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
            // TODO: Play "Click" empty sound?
            Reload(); // Auto-reload for convenience? Or just do nothing.
            return;
        }

        // 2. Decrement Ammo
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
        FlashlightComp->ToggleVisibility();
    }
}

void AFPSCharacter::TakeDamageVS(float DamageAmount)
{
    Health -= DamageAmount;

    if (Health <= 0.0f)
    {
        Health = 0.0f;
        
        UGameplayStatics::OpenLevel(GetWorld(), FName("LoseMenu"));

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
    // How much ammo do we need to fill the clip?
    int32 AmmoNeeded = MaxClipSize - CurrentClipAmmo;

    // Do we have enough in reserve?
    if (MaxReserveAmmo >= AmmoNeeded)
    {
        MaxReserveAmmo -= AmmoNeeded;
        CurrentClipAmmo = MaxClipSize;
    }
    else
    {
        // We don't have enough for a full reload, just put in what's left
        CurrentClipAmmo += MaxReserveAmmo;
        MaxReserveAmmo = 0;
    }

    UE_LOG(LogTemp, Log, TEXT("Reloaded! Clip: %d | Reserve: %d"), CurrentClipAmmo, MaxReserveAmmo);

    if (MyHUD)
    {
        MyHUD->UpdateAmmo(CurrentClipAmmo, MaxReserveAmmo);
    }
}

bool AFPSCharacter::CanFire() const
{
    return CurrentClipAmmo > 0;
}