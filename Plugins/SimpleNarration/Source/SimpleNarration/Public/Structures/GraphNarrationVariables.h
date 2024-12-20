// © 2024 KAG. All rights reserved.

#pragma once
#include "GraphNarrationVariables.generated.h"

USTRUCT()
struct FGraphNarrationVariables
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, Category="GraphNarration")
	TMap<FName, bool> Booleans;

	UPROPERTY(EditAnywhere, Category="GraphNarration")
	TMap<FName, int32> Ints;
};