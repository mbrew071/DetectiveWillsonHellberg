// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TextAnimPreset.generated.h"

UCLASS()
class ACTORNARRATION_API UTextAnimPreset : public UPrimaryDataAsset
{
	GENERATED_BODY()

//	UPROPERTY(BlueprintReadWrite,EditAnywhere)
//	bool bAnimate = true;
//If I dont want to animate then i would put 0.0f in time 

public:	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float SecondsPerCharacter = 0.075f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MasterDelayAtSpecialChar = 0.5f;

	//-1 means we use MasterDekayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtDot = -1.0f;

	//-1 means we use MasterDekayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtComa = -1.0f;

	//-1 means we use MasterDekayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtExclamationMark = -1.0f;

	//-1 means we use MasterDekayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtQuestionMark = -1.0f;
};
