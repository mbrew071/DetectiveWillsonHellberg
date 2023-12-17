// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ClickInteractComponent_I.generated.h"

UINTERFACE(MinimalAPI)
class UClickInteractComponent_I : public UInterface
{
	GENERATED_BODY()
};

class CLICKINTERACTIONSYSTEM_API IClickInteractComponent_I
{
	GENERATED_BODY()

public:
	//InteractingCharacter <=> The Character that interacted with ClickInteractComponent
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TryInteractWith_I(const AActor* InteractingCharacter);
	
};
