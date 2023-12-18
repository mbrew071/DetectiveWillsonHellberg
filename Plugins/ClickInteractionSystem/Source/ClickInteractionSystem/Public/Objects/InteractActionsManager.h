// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractActionsManager.generated.h"

class UInteractActions;

UCLASS()
class CLICKINTERACTIONSYSTEM_API UInteractActionsManager : public UObject
{
	GENERATED_BODY()
	
public:
	static TMap<TSoftClassPtr<UInteractActions>, UInteractActions*> InteractActions;

public:
	UFUNCTION(BlueprintCallable)
	static UInteractActions* GetInteractActions(TSoftClassPtr<UInteractActions> ActionClass);
	
};
