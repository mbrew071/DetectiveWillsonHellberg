// All rights reserved, HelloIT Norweskie

#pragma once
#include "TextSequence.h"
#include "Enumerations/NarrationEndRule.h"
#include "NarrationLine.generated.h"

class UTextAnimPreset;

USTRUCT(BlueprintType)
struct FNarrationLine : public FTableRowBase
{
	GENERATED_BODY()

public:	
	//This regulates when end of the Narration will be called
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TEnumAsByte <ENarrationEndRule> NarrationEndRule = ENarrationEndRule::OnBoth;

	//Regulates delays in between displayed text characters
	//Animations are defined in Data table
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName TextAnimationPreset = "Default";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTextSequence> TextSequences;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TSoftObjectPtr<UTextAnimPreset> TextAnimPreset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<USoundBase> Audio;
};
