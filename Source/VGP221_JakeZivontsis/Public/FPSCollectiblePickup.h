// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSBasePickup.h"
#include "FPSCollectiblePickup.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSCollectiblePickup : public AFPSBasePickup
{
	GENERATED_BODY()

protected:
	virtual void OnPickup(class AFPSCharacter* Player) override;	
};
