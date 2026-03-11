#pragma once

#include "FPSCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBasePickup.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSBasePickup : public AActor
{
	GENERATED_BODY()

public:
	AFPSBasePickup();

protected:
	virtual void BeginPlay() override;

	// The collision sphere
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* CollisionSphere;

	// The visual mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* PickupMesh;

	// Overlap function
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Virtual function that our child classes will override
	virtual void OnPickup(class AFPSCharacter* Player);
};