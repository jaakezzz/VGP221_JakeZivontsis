#include "FPSAmmoPickup.h"

void AFPSAmmoPickup::OnPickup(AFPSCharacter* Player)
{
    Player->AddReserveAmmo(AmmoAmount);
    Destroy();
}
