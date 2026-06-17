#include "FPSHealthPickup.h"

bool AFPSHealthPickup::OnPickup(AFPSCharacter* Player)
{
    // If the player is at or above max health
    if (Player->Health >= Player->MaxHealth)
    {
        return false;
    }

    // The player needs health
    Player->Heal(HealAmount);
    return true;
}