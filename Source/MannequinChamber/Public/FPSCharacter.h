#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h" 
#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MANNEQUINCHAMBER_API AFPSCharacter : public ACharacter
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* ReloadAction;

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
    int32 CurrentClipAmmo = 0; // Currently loaded

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 MaxReserveAmmo = 0; // Current ammo in reserve (not loaded)

	// Score Tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 Collectibles = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 EnemiesKilled = 0;

    // --- AUDIO ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PlayerHurtSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PlayerHealSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float HurtSoundCooldown = 0.67f; // Half a second before you can grunt again

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float HealSoundCooldown = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* GunShotSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* ReloadSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* FlashlightClickSound;

    // --- FUNCTIONS ---
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void TakeDamageVS(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Reload();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AddReserveAmmo(int32 AmmoAmount);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddCollectible();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddEnemyKill();

    // Check if we can fire (Do we have ammo?)
    bool CanFire() const;

    // The timer handle for reloading
    FTimerHandle ReloadTimerHandle;

    // The function that actually gives the ammo when the timer finishes
    void FinishReload();

    // Is the player currently reloading?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsReloading = false;

    // How long the reload takes (in seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float ReloadTime = 0.75f;

 private:
    float LastHurtSoundTime = -100.0f;
    float LastHealSoundTime = -100.0f;
};