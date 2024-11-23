// All rights reserved, 

#pragma once

#include "CoreMinimal.h"
#include "ClickInteractDelegates.h"
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
	UFUNCTION(BlueprintNativeEvent)
	void PerformInteraction(const AActor* InteractingCharacter, const AActor* InteractionTarget, const FGameplayTag InteractionType);

///////////////////////////////////////////////////// DELEGATES ///////////////////////////////////////////
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteractionEnd OnInteractionEnd;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteractionBegin OnInteractionBegin;

	/*UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteractionProgress OnInteractionProgress;*/
};
