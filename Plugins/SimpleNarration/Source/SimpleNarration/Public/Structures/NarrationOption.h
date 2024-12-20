// © 2024 KAG. All rights reserved.

#pragma once
#include "NarrationOption.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNarrationOption
{
 GENERATED_BODY();
public:
 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration")
 FText Text;

 //TODO make note about advancedDIsplay for struct property
 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 bool bVisible = true;

 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 FName TextStyle = "DefaultOption";
};