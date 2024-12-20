// © 2024 KAG. All rights reserved.


#include "FunctionLibraries/SimpleNarrationLibrary.h"
#include "Components/RichTextBlock.h"

void USimpleNarrationLibrary::RichTextStyleRowToTextBlockStyle(const FRichTextStyleRow& RichTextStylRow, FTextBlockStyle& TextBlockStyle)
{
	TextBlockStyle = RichTextStylRow.TextStyle;
}

void USimpleNarrationLibrary::SetRichTextBlockStyle(const FName& TextStyle, URichTextBlock* RichTextBlock)
{
	if(RichTextBlock == nullptr)
	{
		return;
	}
	const UDataTable* DT = RichTextBlock->GetTextStyleSet();

	const FRichTextStyleRow* Style = DT->FindRow<FRichTextStyleRow>(TextStyle,nullptr, false);
	if(Style)
	{
		RichTextBlock->SetDefaultTextStyle(Style->TextStyle);
		return;
	}
	
	const FRichTextStyleRow* StyleDefault = DT->FindRow<FRichTextStyleRow>(FName("Default"),nullptr, false);
	if (StyleDefault)
	{
		RichTextBlock->SetDefaultTextStyle(StyleDefault->TextStyle);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to SetRichTextBlockStyle neither %s OR %s were found in DataTable. Make sure that your RichTextBlock has set DataTable with correct styles"), *TextStyle.ToString(), TEXT("Default"));
}

void USimpleNarrationLibrary::ScreenNarrationToSceneNarration(const TArray<FSceneNarrationPiece>& SceneNarrationPieces,  TArray<FScreenNarrationPiece>& ScreenNarrations)
{
	ScreenNarrations.Empty();
	for (auto& Element : SceneNarrationPieces)
	{
		FScreenNarrationPiece Temp;
		Temp.Speaker = Element.Speaker;
		Temp.Text = Element.Text;
		Temp.Audio = Element.Audio;
		Temp.TextStyle = Element.TextStyle;
		Temp.SpeakerTextStyle = Element.SpeakerTextStyle;

		ScreenNarrations.Add(Temp);
	}
}
