// All rights reserved, HelloIT Norweskie

#pragma once
#include "TextSequence.h"
#include "NarrationLine.generated.h"

class UTextAnimPreset;

USTRUCT(BlueprintType)
struct FNarrationLine : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FTextSequence> TextSequences;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTextAnimPreset> TextAnimPreset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<USoundBase> Audio;
};
