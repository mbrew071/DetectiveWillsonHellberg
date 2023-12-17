// All rights reserved, HelloIT Norweskie


#include "Objects/DetectiveInteractActions.h"
#include "GameplayTagContainer.h"


void UDetectiveInteractActions::PerformInteraction_Implementation(const AActor* OwningActor,
                                                                  const AActor* InteractionTarget,
                                                                  const FGameplayTag InteractionType)
{
	Super::PerformInteraction_Implementation(OwningActor, InteractionTarget, InteractionType);

	//Do Interact logic here by switch on interact tag
	
}
