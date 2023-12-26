// All rights reserved, HelloIT Norweskie


#include "SceneComponents/ActorNarrationComponent.h"

// Sets default values for this component's properties
UActorNarrationComponent::UActorNarrationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorNarrationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorNarrationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActorNarrationComponent::PlayNarration(FNarrationLine NarrationLine)
{
	return;
}

void UActorNarrationComponent::DisruptNarration(FNarrationLine NarrationLine)
{
	return;
}

