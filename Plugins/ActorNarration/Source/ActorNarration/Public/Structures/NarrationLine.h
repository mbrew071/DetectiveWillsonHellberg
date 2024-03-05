// All rights reserved, HelloIT Norweskie

#pragma once
#include "GameplayTagContainer.h"
#include "TextSequence.h"
#include "Enumerations/NarrationEndRule.h"
#include "Engine/DataTable.h"
#include "NarrationLine.generated.h"

class UTextAnimPreset;

USTRUCT(BlueprintType)
struct FNarrationLine : public FTableRowBase
{
	GENERATED_BODY()

public:	
	//This regulates when end of the Narration will be called
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|NarrationLine")
	TEnumAsByte <ENarrationEndRule> NarrationEndRule = ENarrationEndRule::OnBoth;

	//Regulates delays in between displayed text characters
	//Delays are defined in Data table
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="ActorNarration|NarrationLine")
	FGameplayTag DelaysPreset = FGameplayTag::RequestGameplayTag("SceneComponent.ActorNarration.DelaysPreset.Default");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="ActorNarration|NarrationLine")
	TArray<FTextSequence> TextSequences;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="ActorNarration|NarrationLine")
	TSoftObjectPtr<USoundBase> Audio;
};
