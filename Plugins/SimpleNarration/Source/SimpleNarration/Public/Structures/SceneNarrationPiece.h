// © 2024 KAG. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "SceneNarrationPiece.generated.h"

USTRUCT(BlueprintType)
struct FSceneNarrationPiece //Lets not inherit it bcs at this point idk how in future it will differ from FScreenNarrationPiece. So composition here is safer than inheritance since changes won't brake already made narration graphs
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration")
	FText Speaker;
 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration")
	FText Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
	TSoftObjectPtr<UObject> Audio;
	
	//Style of text that will be applied to the spoken text
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
	FName TextStyle = "DefaultScenePiece";

	//Style of text that will be applied to the speaker name 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SimpleNarration", AdvancedDisplay)
	FName SpeakerTextStyle = "DefaultSceneSpeaker";
};
