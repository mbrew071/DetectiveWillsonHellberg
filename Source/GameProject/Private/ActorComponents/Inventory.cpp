// All rights reserved, HelloIT Norweskie


#include "ActorComponents/Inventory.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
}

void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

