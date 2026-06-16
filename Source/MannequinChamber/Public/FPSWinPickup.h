#pragma once

#include "CoreMinimal.h"
#include "FPSBasePickup.h"
#include "FPSWinPickup.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSWinPickup : public AFPSBasePickup
{
	GENERATED_BODY()
	
protected:
	virtual void OnPickup(class AFPSCharacter* Player) override;
};
