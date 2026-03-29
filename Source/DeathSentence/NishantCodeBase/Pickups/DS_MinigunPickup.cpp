#include "NishantCodeBase/Pickups/DS_MinigunPickup.h"
#include "NishantCodeBase/Player/DS_Car.h"
#include "NishantCodeBase/Components/WeaponComponent/WeaponComponent.h"

void ADS_MinigunPickup::ApplyTo(ADS_Car* Car)
{
	if (!Car) return;

	if (UWeaponComponent* WeaponComp = Car->FindComponentByClass<UWeaponComponent>())
	{
		WeaponComp->ActivateMinigun(200);
	}
}