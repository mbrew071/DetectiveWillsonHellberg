// All rights reserved, HelloIT Norweskie

#pragma once
#include "TextAnimRow.generated.h"

USTRUCT()
struct FTextAnimRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	//Time it takes to show next character of the narration
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float SecondsPerCharacter = 0.075f;

	//This delay will be applied at special chars
	//unless delay for given special char has been defined
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float MasterDelayAtSpecialChar = 0.5f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtDot = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtComa = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtExclamationMark = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DelayAtQuestionMark = -1.0f;
};
