// All rights reserved, HelloIT Norweskie

#pragma once
#include "TextSequence.generated.h"

class UTextStylePreset;

USTRUCT(BlueprintType)
struct FTextSequence
{
	GENERATED_BODY()

	//Text sequence's content
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText Text;

	//Regulates style of displayed text sequence. eg Font, Color, Outline
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSoftObjectPtr<UTextStylePreset> TextStylePreset;
};
