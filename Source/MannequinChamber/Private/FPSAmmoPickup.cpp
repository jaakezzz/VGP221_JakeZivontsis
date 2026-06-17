#include "FPSAmmoPickup.h"

bool AFPSAmmoPickup::OnPickup(AFPSCharacter* Player)
{
    Player->AddReserveAmmo(AmmoAmount);
	return true;
}
