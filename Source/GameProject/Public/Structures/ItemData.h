// All rights reserved, HelloIT Norweskie

#pragma once
#include "ItemData.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name = "DefaultName";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;
	
};