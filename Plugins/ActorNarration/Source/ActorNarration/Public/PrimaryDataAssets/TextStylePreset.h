// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TextStylePreset.generated.h"


UCLASS()
class ACTORNARRATION_API UTextStylePreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//Text's font
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FSlateFontInfo SlateFontInfo;

	//Text's color
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FSlateColor SlateColor;
};
