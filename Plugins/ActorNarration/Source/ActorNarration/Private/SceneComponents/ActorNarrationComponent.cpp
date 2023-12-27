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

void UActorNarrationComponent::PlayNarration(FNarrationLine NarrationLine)
{
	ENarrationLineType NarrationLineType = GetNarrationLineType(NarrationLine);

	//Check is any narration currently playing?
	//if yes then cancel previous narration and play a new one
	
	switch (NarrationLineType)
	{
	case ENarrationLineType::Both:
		{
			HandleNarrationBoth(NarrationLine);
		}
	case ENarrationLineType::TextOnly:
		{
			HandleNarrationTextOnly(NarrationLine);
		}
	case ENarrationLineType::AudioOnly:
		{
			HandleNarrationAudioOnly(NarrationLine);
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

void UActorNarrationComponent::HandleNarrationBoth(const FNarrationLine& NarrationLine)
{
	BeginNarration();
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationTextOnly(const FNarrationLine& NarrationLine)
{
	BeginNarration();
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationAudioOnly(const FNarrationLine& NarrationLine)
{
	BeginNarration();
	EndNarration();
}

void UActorNarrationComponent::HandleNarrationEmpty()
{
	UE_LOG(LogTemp, Error, TEXT("Attempted to play an empty narration"));
}

void UActorNarrationComponent::BeginNarration()
{
	//Check is any narration currently playing?
	if (bNarrationOngoing)
	{
		//Cancel previous narration and 
		DisruptNarration();
	}
	//Begin new narration
	bNarrationOngoing = true;
	OnNarrationBegin.Broadcast();
}

void UActorNarrationComponent::EndNarration()
{
	bNarrationOngoing = false;
	OnNarrationEnd.Broadcast();
}

void UActorNarrationComponent::DisruptNarration()
{
	//TODO Fajnie gdyby zzwracało FName tej Narration line ktora zostala przerwana
	//TODO trzeba przemyslec w jaki fajny sposob to osiagnac
	// Moze bNarrationOngoing zamienić na FName z nazwą obecnie odgrywanej narracji?
	// i gdy ten FName jest empty to znaczy ze zadna narracja nie jest obecnie grana
	
	if (!bNarrationOngoing) { return; }
	bNarrationOngoing = false;
	OnNarrationDisrupt.Broadcast();
}


