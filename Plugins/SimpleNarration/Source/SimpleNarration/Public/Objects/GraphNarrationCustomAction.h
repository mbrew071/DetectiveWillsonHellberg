// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GraphNarrationCustomAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomActionEnd, const bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomActionBegin);

/**
 * Code to executed when Custom action takes place
 * will be implemented by Children of this class.
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class SIMPLENARRATION_API UGraphNarrationCustomAction : public UObject
{
	GENERATED_BODY()

	virtual UWorld* GetWorld() const override;
	
public:	
	//Override this function.
	UFUNCTION(BlueprintNativeEvent)
	void PerformCustomAction(const UObject* Graph, const UObject* Widget, const FGameplayTag ActionTag, const UObject* DTO);

	///////////////////////////////////////////////////// DELEGATES ///////////////////////////////////////////
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCustomActionEnd OnInteractionEnd;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCustomActionBegin OnInteractionBegin;
};
