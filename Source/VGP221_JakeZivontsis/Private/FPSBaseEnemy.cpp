// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "FPSCharacter.h" // Needed to damage the player

// Sets default values
AFPSBaseEnemy::AFPSBaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFPSBaseEnemy::OnHit);

	// Set the capsule to block other Pawns (the player)
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// This ensures the Hit event will actually fire
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AFPSBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBaseEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this))
	{
		// Check if the thing that bumped into us is the Player
		if (AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor))
		{
			// Get the current game time
			float CurrentTime = GetWorld()->GetTimeSeconds();

			// Check if enough time has passed since the last hit
			if (CurrentTime - LastDamageTime >= DamageCooldown)
			{
				// Deal the damage
				Player->TakeDamageVS(TouchDamage);

				// Update the timestamp
				LastDamageTime = CurrentTime;

				// Optional: Push the player back slightly so they don't get "stuck"
				FVector PushDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				Player->LaunchCharacter(PushDirection * 500.0f, true, true);
			}
		}
	}
}

void AFPSBaseEnemy::TakeEnemyDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		Destroy();
	}
}

// Called every frame
void AFPSBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSBaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

