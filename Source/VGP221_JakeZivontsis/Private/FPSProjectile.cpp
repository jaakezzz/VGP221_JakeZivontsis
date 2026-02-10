#include "FPSProjectile.h"

AFPSProjectile::AFPSProjectile()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);

    CollisionComp->SetCollisionProfileName(TEXT("Projectile")); // 1. Sets physics to "Block" walls
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)); // 2. Prevents it from "sliding" up walls
    CollisionComp->CanCharacterStepUpOn = ECB_No; // 3. Prevents players from walking on bullets

    CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
    RootComponent = CollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    InitialLifeSpan = 3.0f; // Die after 3 seconds
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this))
    {
        // Only push objects that are actually simulating physics (like crates/barrels)
        if (OtherComp && OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        }
        Destroy();
    }
}