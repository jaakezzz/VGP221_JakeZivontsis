#include "FPSCollectiblePickup.h"

bool AFPSCollectiblePickup::OnPickup(AFPSCharacter* Player)
{
    Player->AddCollectible();
	return true;
}