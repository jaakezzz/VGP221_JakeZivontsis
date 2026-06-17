#pragma once

#include "CoreMinimal.h"
#include "FPSBasePickup.h"
#include "FPSWinPickup.generated.h"

UCLASS()
class MANNEQUINCHAMBER_API AFPSWinPickup : public AFPSBasePickup
{
	GENERATED_BODY()
	
protected:
	virtual bool OnPickup(class AFPSCharacter* Player) override;
};
