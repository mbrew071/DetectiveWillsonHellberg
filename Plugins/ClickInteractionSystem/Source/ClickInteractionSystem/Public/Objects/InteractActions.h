// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "InteractActions.generated.h"

/*
 * Code executed when interaction takes place
 * will be implemented by Children of this class.
 */
UCLASS(Blueprintable, BlueprintType)
class CLICKINTERACTIONSYSTEM_API UInteractActions : public UObject
{
	GENERATED_BODY()

public:	
	//Override this function.
	UFUNCTION(Blueprintable)
	virtual void PreformInteraction(const AActor* OwningActor, const AActor* InteractionTarget, const FName Tag,
	                                bool& bSuccess);

	//TODO On Interaction finished
	//TODO Create interface for calling PerformInteraction
};
