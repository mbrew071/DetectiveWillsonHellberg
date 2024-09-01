// All rights reserved hehe

#pragma once
#include "GameplayTagContainer.h"
#include "Framework/Commands/InputChord.h"
#include "InputChordSettings.generated.h"

USTRUCT(BlueprintType)
struct FInputChordSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="GameSettings")
	TMap<FGameplayTag, FInputChord> Inputs;
	
};
