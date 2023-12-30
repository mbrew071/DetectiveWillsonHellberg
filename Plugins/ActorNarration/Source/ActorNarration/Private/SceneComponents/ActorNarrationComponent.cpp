// All rights reserved, HelloIT Norweskie


#include "SceneComponents/ActorNarrationComponent.h"

/////////////////////////////////////// GENERAL ///////////////////////////////////////////////////////
// Sets default values for this component's properties
UActorNarrationComponent::UActorNarrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

// Called when the game starts
void UActorNarrationComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UActorNarrationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

///////////////////////////////////// Narration ///////////////////////////////////////////////////////////////

void UActorNarrationComponent::PlayNarration(FNarrationLine NarrationLine, FName NarrationName)
{
	ENarrationLineType NarrationLineType = GetNarrationLineType(NarrationLine);

	//Check is any narration currently playing?
	//if yes then cancel previous narration and play a new one
	
	switch (NarrationLineType)
	{
	case ENarrationLineType::Both:
		{
			HandleNarrationBoth(NarrationLine, NarrationName);
		}
	case ENarrationLineType::TextOnly:
		{
			HandleNarrationTextOnly(NarrationLine, NarrationName);
		}
	case ENarrationLineType::AudioOnly:
		{
			HandleNarrationAudioOnly(NarrationLine, NarrationName);
		}
	case ENarrationLineType::Empty:
		{
			HandleNarrationEmpty();
		}
	}
}

ENarrationLineType UActorNarrationComponent::GetNarrationLineType(const FNarrationLine& NarrationLine)
{
	if (!NarrationLine.Audio.IsValid() && NarrationLine.TextSequences.IsEmpty())
	{
		// This Narration line doesnt have Audio and text
		return ENarrationLineType::Empty;
	}

	if (NarrationLine.Audio.IsValid() && !NarrationLine.TextSequences.IsEmpty())
	{
		// This Narration line has both audio and text
		return ENarrationLineType::Both;
	}
	
	if (NarrationLine.Audio.IsValid() && NarrationLine.TextSequences.IsEmpty())
	{
		// This Narration line has only Audio
		return ENarrationLineType::AudioOnly;
	}
	
	if (!NarrationLine.Audio.IsValid() && !NarrationLine.TextSequences.IsEmpty())
	{
		// This Narration line has only Text
		return ENarrationLineType::TextOnly;
	}
	
	UE_LOG(LogTemp, Fatal, TEXT("Error. Failed to find NarrationLineType | ActorNarrationComponent"));
	return ENarrationLineType::Empty;
}

void UActorNarrationComponent::HandleNarrationBoth(const FNarrationLine& NarrationLine, FName& NarrationName)
{
	BeginNarration(NarrationName);
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationTextOnly(const FNarrationLine& NarrationLine, FName& NarrationName)
{
	BeginNarration(NarrationName);
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationAudioOnly(const FNarrationLine& NarrationLine, FName& NarrationName)
{
	BeginNarration(NarrationName);
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationEmpty()
{
	UE_LOG(LogTemp, Error, TEXT("Attempted to play an empty narration"));
}

void UActorNarrationComponent::BeginNarration(FName NarrationName)
{
	//Check is any narration currently playing?
	if (!OngoingNarration.IsNone())
	{
		//Cancel previous narration and 
		DisruptNarration();
	}
	//Begin new narration
	OngoingNarration = NarrationName;
	
	OnNarrationBegin.Broadcast();
}

void UActorNarrationComponent::EndNarration()
{
	OngoingNarration = "None";
	
	OnNarrationEnd.Broadcast();
}

void UActorNarrationComponent::NarrationLoop(const FNarrationLine& NarrationLine)
{
	//index of current Sequence
	//int32 SequenceIndex = 0;
	//index of current word in that sequence
	int32 CharacterIndex = 0;

	for (auto& Element : NarrationLine.TextSequences)
	{
		//FText Text = FText();
		FString Text;
		//foreach char in text sequence
		
		for (auto& Char : Text.GetCharArray())
		{
			// get delay for the char
			// Wait
			
			// append
		//	Text.Append(static_cast<TChar>(Char));

			//Call interface fucntion 
		}
	}
}

void UActorNarrationComponent::DisruptNarration()
{
	if (OngoingNarration.IsNone()) { return; }
	OnNarrationDisrupt.Broadcast(OngoingNarration);
	OngoingNarration = "None";
}


