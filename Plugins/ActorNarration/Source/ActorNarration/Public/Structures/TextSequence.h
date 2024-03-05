// All rights reserved, HelloIT Norweskie

#pragma once
#include "GameplayTagContainer.h"
#include "TextSequence.generated.h"

class UTextStylePreset;

USTRUCT(BlueprintType)
struct FTextSequence
{
	GENERATED_BODY()

	//Regulates style of displayed text sequence. eg Font, Color, Outline
	//Styles are defined in Data table
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|TextSequence")
	FGameplayTag TextStylePreset = FGameplayTag::RequestGameplayTag("SceneComponent.ActorNarration.TextStyle.Roboto.Regular.White");
	
	//Text sequence's content
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "ActorNarration|TextSequence")
	FText Text;
	
};
