// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSBaseEnemy.generated.h"
class AAIController;

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSBaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSBaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Health and Combat Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float TouchDamage = 20.0f;

	// Collision Function (When Player touches Enemy)
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// --- AI & Chase Variables ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ChaseDelay = 2.0f; // Seconds the player must look

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	float CurrentLookTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsChasing = false;

	// Reference to the controller that will pilot the enemy
	UPROPERTY()
	AAIController* EnemyAIController;

private:
	// Time (in seconds) when damage was last dealt
	float LastDamageTime = 0.0f;

	// How often (in seconds) the enemy can deal damage
	float DamageCooldown = 0.25f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Take Damage Function (When Projectile hits Enemy)
	void TakeEnemyDamage(float DamageAmount);

};
