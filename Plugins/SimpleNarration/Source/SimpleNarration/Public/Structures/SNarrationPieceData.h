// © 2024 KAG. All rights reserved.

#pragma once
#include "GameplayTagContainer.h"
#include "SNarrationPieceData.generated.h"

/*
 * This struct adds some queryable data to SimpleNarration options
 */
USTRUCT(BlueprintType)
struct FSNarrationPieceData
{
	GENERATED_BODY()
public:
	//Number of times player character chose this dialogue option
	//if player character hasn't chosen this option then it will be 0
	UPROPERTY(EditAnywhere, Category="SimpleNarration")
	uint8 ChoseCounter = 0;

	//SimpleNarration.OptionType.Default
	//eg. when this is a quest related option then We will use  
	UPROPERTY(EditAnywhere, Category="SimpleNarration")
	FGameplayTag Type = FGameplayTag::RequestGameplayTag("SimpleNarration.OptionType.Default", false);
};
