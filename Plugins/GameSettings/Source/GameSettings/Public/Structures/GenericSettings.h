// All rights reserved KAG

#pragma once
#include "GameplayTagContainer.h"
#include "GenericSettings.generated.h"
/**
 * Structure that contains maps of custom game settings 
 */
USTRUCT(BlueprintType)
struct FGenericSettings
{
	GENERATED_BODY()

	//Settings such as "Display blood"
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Custom")
	TMap<FGameplayTag, bool> BoolSettings;

	//Settings such as "Hours per day"
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Custom")
	TMap<FGameplayTag, int32> IntSettings;

	//Setting such as "Music volume"
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Custom")
	TMap<FGameplayTag, float> FloatSettings;

};
