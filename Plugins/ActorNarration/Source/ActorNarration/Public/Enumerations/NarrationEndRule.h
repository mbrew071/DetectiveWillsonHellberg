// All rights reserved, HelloIT Norweskie

#pragma once
#include "NarrationEndRule.generated.h"

UENUM(BlueprintType)
enum ENarrationEndRule : uint8 
{
	//Both Audio and Text animation has to finish to End the narration.
	OnBoth,
	
	//Narration will end when Audio finishes.
	OnAudio,

	//Narration will end when Text animation finishes.  
	OnText,

	//Narration will end when Text animation finishes.
	//If Audio ends and text anim is still ongoing
	//then the audio will be looped
	OnTextWhileAudioLooped,
};
