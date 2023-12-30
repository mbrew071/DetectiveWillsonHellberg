// All rights reserved, HelloIT Norweskie

#pragma once
#include "TextSequence.generated.h"

class UTextStylePreset;

USTRUCT(BlueprintType)
struct FTextSequence
{
	GENERATED_BODY()

	//Regulates style of displayed text sequence. eg Font, Color, Outline
	//Styles are defined in Data table
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName TextStylePreset = "Default";
	
	//Text sequence's content
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText Text;

	//Regulates style of displayed text sequence. eg Font, Color, Outline
	//UPROPERTY(BlueprintReadWrite,EditAnywhere)
	//TSoftObjectPtr<UTextStylePreset> TextStylePreset;


};
