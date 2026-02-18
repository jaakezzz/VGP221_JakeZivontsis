#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h" 
#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFPSCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* FirstPersonCameraComponent;

    // Gun Mesh (Attached to camera)
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    UStaticMeshComponent* GunMesh;

	// Flashlight Component 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    class USpotLightComponent* FlashlightComp;

    // Input Actions (Assign these in Blueprint later)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* FlashlightAction;

    // Projectile class to spawn
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AFPSProjectile> ProjectileClass;

    // The Blueprint Widget Class to spawn (Assign WBP_GameHUD here in Editor)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    // Add a reference to the specific C++ HUD class
    UPROPERTY()
    class UFPSHUDWidget* MyHUD;

protected:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Fire();
    void ToggleFlashlight();

public:
    // --- STATS ---

    // Health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    float Health = 100.0f;

    // Ammo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 MaxClipSize = 7; // How many bullets in the gun

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 CurrentClipAmmo = 7; // Currently loaded

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 MaxReserveAmmo = 1000; // The "Insane amount" you requested

    // --- FUNCTIONS ---

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TakeDamageVS(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Reload();

    // Check if we can fire (Do we have ammo?)
    bool CanFire() const;

};