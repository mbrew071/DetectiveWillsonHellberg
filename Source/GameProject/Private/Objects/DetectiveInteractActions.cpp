// All rights reserved, HelloIT Norweskie

#include "Objects/DetectiveInteractActions.h"
#include "GameplayTagContainer.h"

void UDetectiveInteractActions::PerformInteraction_Implementation(const AActor* InteractingCharacter,
                                                                  const AActor* InteractionTarget,
                                                                  const FGameplayTag InteractionType)
{
	Super::PerformInteraction_Implementation(InteractingCharacter, InteractionTarget, InteractionType);
	//Do Interact logic here by switch on interact tag
}
