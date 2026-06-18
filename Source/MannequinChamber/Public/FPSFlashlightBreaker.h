#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FPSFlashlightBreaker.generated.h"

UCLASS()
class MANNEQUINCHAMBER_API AFPSFlashlightBreaker : public AActor
{
    GENERATED_BODY()

public:
    AFPSFlashlightBreaker();

protected:
    // The collision volume
    UPROPERTY(VisibleDefaultsOnly, Category = "Trigger")
    UBoxComponent* CollisionBox;

    // The overlap event function
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};