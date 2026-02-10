#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"

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

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
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