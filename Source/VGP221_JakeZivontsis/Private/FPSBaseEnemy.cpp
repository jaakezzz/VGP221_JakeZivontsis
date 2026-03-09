// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "FPSCharacter.h" // Needed to damage the player
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"

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
	
	// Get the AI Controller assigned to this character
	EnemyAIController = Cast<AAIController>(GetController());
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

	// Get the player
	AFPSCharacter* Player = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player) return;

	if (!bIsChasing)
	{
		bool bIsBeingIlluminated = false;

		// 1. Check if the player's flashlight exists and is turned ON
		if (USpotLightComponent* Flashlight = Player->FlashlightComp)
		{
			if (Flashlight->IsVisible())
			{
				// 2. Check Distance (Is the enemy close enough to be hit by the light?)
				float DistanceToPlayer = FVector::Distance(GetActorLocation(), Flashlight->GetComponentLocation());

				if (DistanceToPlayer <= Flashlight->AttenuationRadius)
				{
					// 3. Check Angle (Is the enemy inside the 33-degree light cone?)
					FVector DirToEnemy = (GetActorLocation() - Flashlight->GetComponentLocation()).GetSafeNormal();
					FVector FlashlightForward = Flashlight->GetForwardVector();

					float LookDot = FVector::DotProduct(FlashlightForward, DirToEnemy);

					// Convert the flashlight's cone angle (degrees) to a dot product threshold (-1 to 1)
					float ConeThreshold = FMath::Cos(FMath::DegreesToRadians(Flashlight->OuterConeAngle));

					if (LookDot >= ConeThreshold)
					{
						bIsBeingIlluminated = true;
					}
				}
			}
		}

		// 4. Handle the Chase Timer
		if (bIsBeingIlluminated)
		{
			CurrentLookTime += DeltaTime; // Accumulate time

			if (CurrentLookTime >= ChaseDelay)
			{
				bIsChasing = true; // Trigger the chase!
			}
		}
		else
		{
			// Reset the timer if the player turns off the light or looks away
			CurrentLookTime = 0.0f;
		}
	}
	else
	{
		// 2. Chasing State
		if (EnemyAIController)
		{
			// Tell the AI to move to the player. 
			// 0.0f is the acceptance radius (how close it needs to get to stop)
			// false is to tell the AI not to stop on overlap (we want it to keep trying to get right on top of the player)
			EnemyAIController->MoveToActor(Player, 0.0f, false);
		}
	}
}

// Called to bind functionality to input
void AFPSBaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

