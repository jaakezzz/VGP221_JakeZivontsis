#pragma once

#include "CoreMinimal.h"
#include "FPSBasePickup.h"
#include "FPSAmmoPickup.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSAmmoPickup : public AFPSBasePickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	int32 AmmoAmount = 30;

protected:
	virtual void OnPickup(class AFPSCharacter* Player) override;
};
