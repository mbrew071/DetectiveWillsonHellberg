// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "NarrationGraph_I.generated.h"

UINTERFACE(MinimalAPI)
class UNarrationGraph_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLENARRATION_API INarrationGraph_I
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void PlayScreenNarrationCompleted();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void PlayScreenNarrationFailed(const FGameplayTag& FailureReason);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void PlaySceneNarrationCompleted();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void PlaySceneNarrationFailed(const FGameplayTag& FailureReason);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void DisplayOptionsCompleted(const int32& ChosenOptions);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void DisplayOptionsFailed(const FGameplayTag& FailureReason);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void ActionCustomCompleted();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration")
	void ActionCustomFailed(const FGameplayTag& FailureReason);

};
