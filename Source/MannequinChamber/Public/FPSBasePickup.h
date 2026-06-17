#pragma once

#include "FPSCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBasePickup.generated.h"

UCLASS()
class MANNEQUINCHAMBER_API AFPSBasePickup : public AActor
{
	GENERATED_BODY()

public:
	AFPSBasePickup();

	// --- AUDIO ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* PickupSound;

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

	// Virtual function that returns TRUE if the item was successfully consumed
	virtual bool OnPickup(class AFPSCharacter* Player);
};