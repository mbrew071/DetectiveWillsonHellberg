// All rights reserved, HelloIT Norweskie


#include "Objects/InteractActions.h"

#include "GameplayTagContainer.h"


void UInteractActions::PerformInteraction_Implementation(const AActor* InteractingCharacter, const AActor* InteractionTarget,
                                                         const FGameplayTag InteractionType)
{
	OnInteractionBegin.Broadcast();
	// Implement this by children
}

