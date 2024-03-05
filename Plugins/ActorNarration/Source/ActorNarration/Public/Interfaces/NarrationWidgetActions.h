// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Structures/NarrationLine.h"
#include "UObject/Interface.h"
#include "SlateBasics.h"
#include "Components/RichTextBlockDecorator.h"
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
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActorNarration|Narration")
	void BeginNarration(const UDataTable* Styles, const TArray<TSubclassOf<URichTextBlockDecorator>>& HardClassArray);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActorNarration|Narration")
	void EndNarration(const bool bSuccess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActorNarration|Narration")
	void DisruptNarration();

	//This sends an info to the UI that narration text should be displayed
	//This gets called for each character in Narration, There is delay between calls
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ActorNarration|Narration")
	void UpdateNarration(const FString& Text);
};
