#include "FPSFlashlightBreaker.h"
#include "FPSCharacter.h" // Needed to cast to the player

AFPSFlashlightBreaker::AFPSFlashlightBreaker()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create a Box Component so we can stretch it across the whole hallway
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    // Set a default size (this can be scaled freely in the Editor)
    CollisionBox->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));

    // Set collision so it only acts as an overlap trigger
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

    // Bind the overlap event
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSFlashlightBreaker::OnOverlapBegin);
}

void AFPSFlashlightBreaker::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this))
    {
        // Check if the thing walking through is the Player
        if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
        {
            // Start the glitch sequence
            Player->StartFlashlightGlitch();

            // Destroy this trigger so it doesn't trigger again
            Destroy();
        }
    }
}