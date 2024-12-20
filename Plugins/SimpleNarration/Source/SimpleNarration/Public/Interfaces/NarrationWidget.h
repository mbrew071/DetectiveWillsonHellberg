// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structures/NarrationOption.h"
#include "UObject/Interface.h"
#include "Structures/ScreenNarrationPiece.h"
#include "GameplayTagContainer.h"
#include "NarrationWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNarrationWidget : public UInterface
{
	GENERATED_BODY()
};

class SIMPLENARRATION_API INarrationWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration|Widget")
	void PlayScreenNarration(const UObject* Graph, const TArray<FScreenNarrationPiece>& Sequences); 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration|Widget")
	void PlaySceneNarration(const UObject* Graph, const TArray<FSceneNarrationPiece>& Sequences); 
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration|Widget")
	void DisplayOptions(const UObject* Graph, const TArray<FNarrationOption>& Options, const float& DecisionTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GraphNarration|Widget")
	void EndNarration();
};
