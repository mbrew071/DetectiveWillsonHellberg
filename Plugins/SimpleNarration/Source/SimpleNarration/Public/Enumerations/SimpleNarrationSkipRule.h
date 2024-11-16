// © 2024 KAG. All rights reserved.

#pragma once

//#include "CoreMinimal.h"

UENUM(BlueprintType)
enum ESimpleNarrationSkipRule
{
	Default,
	
	PressOrDuration,

	Press,
	
	Duration,
	
};