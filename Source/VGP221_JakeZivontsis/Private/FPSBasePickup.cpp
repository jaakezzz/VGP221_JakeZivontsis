#include "FPSBasePickup.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"

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
			// Call the virtual function
			OnPickup(Player);
		}
	}
}

void AFPSBasePickup::OnPickup(AFPSCharacter* Player)
{
	// virtual function to be overridden by child classes
}