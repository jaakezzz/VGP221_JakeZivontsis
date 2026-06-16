#pragma once

#include "CoreMinimal.h"
#include "FPSBasePickup.h"
#include "FPSHealthPickup.generated.h"

UCLASS()
class VGP221_JAKEZIVONTSIS_API AFPSHealthPickup : public AFPSBasePickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	float HealAmount = 25.0f;

protected:
	virtual void OnPickup(class AFPSCharacter* Player) override;
};
