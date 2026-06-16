#include "FPSCollectiblePickup.h"

void AFPSCollectiblePickup::OnPickup(AFPSCharacter* Player)
{
    Player->AddCollectible();
    Destroy();
}