// © 2024 KAG. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structures/SceneNarrationPiece.h"
#include "Structures/ScreenNarrationPiece.h"
#include "SimpleNarrationLibrary.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class SIMPLENARRATION_API USimpleNarrationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="SimpleNarration", BlueprintPure)
	static void RichTextStyleRowToTextBlockStyle(const FRichTextStyleRow& RichTextStylRow, FTextBlockStyle& TextBlockStyle);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration")
	static void SetRichTextBlockStyle(const FName& TextStyle, URichTextBlock* RichTextBlock); 

	UFUNCTION(BlueprintCallable, Category="SimpleNarration", BlueprintPure)
	static void ScreenNarrationToSceneNarration(const TArray<FSceneNarrationPiece>& SceneNarrationPieces,  TArray<FScreenNarrationPiece>& ScreenNarrations); 

};
