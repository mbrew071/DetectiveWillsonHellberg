// © 2024 KAG. All rights reserved.

#pragma once
#include "Enumerations/SimpleNarrationSkipRule.h"
#include "ScreenNarrationPiece.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FScreenNarrationPiece
{
 GENERATED_BODY();

 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration")
 FText Speaker;
 
 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration")
 FText Text;

 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 TSoftObjectPtr<UObject> Audio;

 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 TEnumAsByte<ESimpleNarrationSkipRule> SkipRuleOverride;

 //Style of text that will be applied to the spoken text
 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 FName TextStyle = "DefaultPhrase";

 //Style of text that will be applied to the speaker name 
 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
 FName SpeakerTextStyle = "DefaultSpeaker";
};