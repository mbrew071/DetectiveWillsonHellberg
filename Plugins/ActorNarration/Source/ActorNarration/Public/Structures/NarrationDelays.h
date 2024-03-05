// All rights reserved, HelloIT Norweskie

#pragma once
#include "Engine/DataTable.h"
#include "NarrationDelays.generated.h"

USTRUCT()
struct FNarrationDelays : public FTableRowBase
{
	GENERATED_BODY()
public:

	//When true the text's animation will finish at the same time as 
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	bool bUseAudioTime = false;
	
	//Time it takes to show next character of the narration
	//Alnum characters will get this delay. eg. 'a' or '1' or 'B' etc..
	//When this <= 0 then Narration will have no animation
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float MasterDelayAtAlnumChar = 0.04f;

	//This delay will be applied at special chars.
	//unless delay for given special char has been defined
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float MasterDelayAtSpecialChar = 0.1f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float DelayAtDot = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float DelayAtComa = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float DelayAtExclamationMark = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float DelayAtQuestionMark = -1.0f;

	//-1 means we use MasterDelayAtSpecialChar
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float DelayAtSpace = -1.0f;

	//When Narration has just finished this is the time until the narration gets ended
	//Useful when I dont want to end narration immediately after the last character of the text has been displayed.
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|Delay")
	float OutroDelay = 2.0f;
};
inline void GetS()
{
	
}
