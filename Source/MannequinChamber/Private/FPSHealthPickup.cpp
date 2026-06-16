#include "FPSHealthPickup.h"

void AFPSHealthPickup::OnPickup(AFPSCharacter* Player)
{
    Player->Heal(HealAmount);
    Destroy();
}
