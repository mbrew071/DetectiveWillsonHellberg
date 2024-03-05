// All rights reserved, HelloIT Norweskie

#pragma once
#include "NarrationLineType.generated.h"

UENUM(BlueprintType)
enum ENarrationLineType : uint8
{
	//We have Both audio and text
	Both,
	
	//We have text sequences but not audio
	TextOnly,
	
	//We have Audio file but no text sequences
	AudioOnly
	
};
