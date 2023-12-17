// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Objects/InteractActions.h"
#include "DetectiveInteractActions.generated.h"

UCLASS()
class GAMEPROJECT_API UDetectiveInteractActions : public UInteractActions
{
	GENERATED_BODY()
	
	void PerformInteraction_Implementation(const AActor* InteractingCharacter, const AActor* InteractionTarget,
	                                       const FGameplayTag InteractionType) override;
};
