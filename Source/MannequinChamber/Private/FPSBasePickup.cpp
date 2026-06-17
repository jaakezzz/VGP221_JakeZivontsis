#include "FPSBasePickup.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

AFPSBasePickup::AFPSBasePickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionSphere;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(CollisionSphere);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Mesh shouldn't block player
}

void AFPSBasePickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBasePickup::OnOverlapBegin);
}

void AFPSBasePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
		{
			// If the child class successfully gets picked up
			if (OnPickup(Player))
			{
				// Play the sound
				if (PickupSound)
				{
					UGameplayStatics::PlaySound2D(this, PickupSound);
				}
				Destroy();
			}
		}
	}
}

// Default base behavior just in case
bool AFPSBasePickup::OnPickup(AFPSCharacter* Player)
{
	return false;
}