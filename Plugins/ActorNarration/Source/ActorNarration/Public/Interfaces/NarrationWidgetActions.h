// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Structures/NarrationLine.h"
#include "UObject/Interface.h"
#include "NarrationWidgetActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNarrationWidgetActions : public UInterface
{
	GENERATED_BODY()
};

class ACTORNARRATION_API INarrationWidgetActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginNarration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndNarration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisruptNarration();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateNarration(FNarrationLine NarrationLine);
};
