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

protected:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Fire();
    void ToggleFlashlight();
};