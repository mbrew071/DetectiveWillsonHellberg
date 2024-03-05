// All rights reserved

#include "SceneComponents/ActorNarrationComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/NarrationWidgetActions.h"
#include "Structures/MasterNarration.h"
#include "TimerManager.h"
#include "Engine/DataTable.h"
#include "Subsystems/ActorNarrationController.h"

/////////////////////////////////////// GENERAL ///////////////////////////////////////////////////////
UActorNarrationComponent::UActorNarrationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	InitWidgetComponent();
}

void UActorNarrationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActorNarrationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

///////////////////////////////////// Narration //////////////////////////////////////////////////////////////

bool UActorNarrationComponent::IsNarrationOngoing()
{
	return !OngoingNarration.IsNone();
}

bool UActorNarrationComponent::PlayNarrationMasterDT(FName DTName, FName NarrationLineName)
{
	//Get Master Narration DT. A DT that contains list of narrations.
	UActorNarrationController* ActorNarrationController = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UActorNarrationController>();
	
	//Get DT that contains Narration line
	TSoftObjectPtr<UDataTable> DataTable;
	if (!GetNarrationLineDT(ActorNarrationController->MasterNarrationDT, DataTable, DTName)) { return false; } 

	// Get Narration line from DT
	FNarrationLine* NarrationLine = nullptr;
	if (!GetNarrationLineFromDT(DataTable, NarrationLine, NarrationLineName)) { return false; }

	//TODO Tu chyba trzeba wywolac raczej  PlayNarrationStruct(); zamiast HandleNarrationProcess 
	//Start Narration process
	return HandleNarrationProcess(NarrationLine, NarrationLineName);
}

bool UActorNarrationComponent::PlayNarration(const TSoftObjectPtr<UDataTable>& DataTable, FName NarrationLineName)
{
	//Get Narration Line
	FNarrationLine* NarrationLine = nullptr;
	if (!GetNarrationLineFromDT(DataTable, NarrationLine, NarrationLineName)) {	return false; }

	if (!NarrationLine)
	{
		UE_LOG(LogTemp,Warning,TEXT("Didnt get the narration line from DT "));
	}

	//TODO Tu chyba trzeba wywolac raczej  PlayNarrationStruct(); zamiast HandleNarrationProcess 
	//Start Narration process
	return HandleNarrationProcess(NarrationLine, NarrationLineName);
}

bool UActorNarrationComponent::PlayNarrationStruct(const FNarrationLine& NarrationLine, FName NarrationLineName)
{
	//TODO finish this later, test for some errors regarding not full struct

	//TODO CREATE A NOTE, Function accepts a PTR, however when I do a BP callable function (in .h) that has input arg const FNarrationLine* NarrationLine the code wont compile SO in that situation I need to make my .h function to intake  const FNarrationLine& NarrationLine and then in the function that accepts the PTR I need to do this: &NarrationLine
	return HandleNarrationProcess(&NarrationLine, NarrationLineName);
}

bool UActorNarrationComponent::HandleNarrationProcess(const FNarrationLine* NarrationLine, FName NarrationLineName)
{
	if(!NarrationLine) { UE_LOG(LogTemp, Error,TEXT("NarrationLine is invalid")); return false; }
	
	//Does this component have a valid widget component class (Class of a widget that will display text) 
	if (!ValidateWidgetClass()) { return false; }
	
	if(IsNarrationOngoing())
	{
		UE_LOG(LogTemp,Warning,TEXT("KGTest 1"));
		//Narration is currently playing therefore disrupt it
		DisruptNarration();
	}

	//create widget
	if (!SetWidgetVisibility(true)) { UE_LOG(LogTemp, Error, TEXT("Failed to create a widget")); return false;}
	
	//Validate narration widget
	if (WidgetComponent == nullptr) { UE_LOG(LogTemp,Error,TEXT("WidgetComponent is nullptr")); return false; }
	
	//Is the narration to be played empty?. (empty means that both Audio and Text is missing)
	if (NarrationLine->TextSequences.Num()<1 && NarrationLine->Audio.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("NarrationLine '%s' has no Audio and no text. At %s "), *NarrationLineName.ToString(), *GetOwner()->GetName());
		return false;
	}
	
	//TODO if yes then cancel previous narration and play a new one
	
	InitBeginNarration(NarrationLineName, NarrationLine->NarrationEndRule);
	
	StartTextNarration(*NarrationLine);
	
	//Start playing Audio. Loop audio only when End rule is OnTextWhileAudioLooped
	StartAudioNarration(NarrationLine->Audio, NarrationLine->NarrationEndRule == OnTextWhileAudioLooped);
	
	return true;
}

///////////////////////////////////// Narration state actions  //////////////////////////////////////////////////
void UActorNarrationComponent::InitBeginNarration(FName NarrationName, const ENarrationEndRule& NarrationEndRule)
{
	//Check is any narration currently playing?
	if (IsNarrationOngoing())
	{
		//Cancel previous narration 
		EndNarration(false);
		//DisruptNarration();
	}
	//Begin new narration
	CurrentNarrationEndRule = NarrationEndRule;
	OngoingNarration = NarrationName;
	OnNarrationBegin.Broadcast();
}

void UActorNarrationComponent::EndNarration(bool bSuccess)
{
	if (!IsNarrationOngoing()) {  return;  }
	
	if (TimerHandleText.IsValid())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandleText);
		TimerHandleText.Invalidate();
	}
	if (TimerHandleAudio.IsValid())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandleAudio);
		TimerHandleAudio.Invalidate();
	}
	
	OngoingNarration = "None";
	TextLoopIndex = 0;

	//return to default end rule 
	CurrentNarrationEndRule = ENarrationEndRule::OnBoth;

	if (IsValid(WidgetComponent))
	{
		SetWidgetVisibility(false);
	}
	if (IsValid(AudioComponent))
	{
		//TODO Change this. We cannot destory component wo only want to stop audio
		/*AudioComponent->DestroyComponent(false);
		AudioComponent = nullptr;*/
	}
	CurrentTextSequences.Empty();
	
	OnNarrationEnd.Broadcast(bSuccess);
	
}

void UActorNarrationComponent::DisruptNarration()
{
	//do nothing if narration is not ongoing
	if (!IsNarrationOngoing()) { return; }
	EndNarration(false);
}

void UActorNarrationComponent::SkipNarration()
{
	//do nothing if narration is not ongoing
	if (!IsNarrationOngoing()) { return; }
	EndNarration(true);
}

void UActorNarrationComponent::NarrationElementFinished(FTimerHandle& TimerHandle)
{
	if (TimerHandle.IsValid())
	{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
	}
	
	switch (CurrentNarrationEndRule)
	{
	case ENarrationEndRule::OnBoth:
		{
			if (!TimerHandleText.IsValid() && !TimerHandleAudio.IsValid())
			{
				EndNarrationWithOutroDelay();
				return;
			}
		}
	case ENarrationEndRule::OnText:
		{
			if (!TimerHandleText.IsValid())
			{
				EndNarrationWithOutroDelay();
				return;
			}
		}
	case ENarrationEndRule::OnAudio:
		{
			if (!TimerHandleAudio.IsValid())
			{
				EndNarrationWithOutroDelay();
				return;
			}
		}
	case ENarrationEndRule::OnTextWhileAudioLooped:
		{
			if (!TimerHandleText.IsValid())
			{
				EndNarrationWithOutroDelay();
				return;
			}
		}
	}
}

void UActorNarrationComponent::EndNarrationWithOutroDelay()
{
	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
	
	TimerDelegate.BindLambda([&]()
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle);
		TimerHandle.Invalidate();
		
		EndNarration(true);
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, NarrationDelays.OutroDelay, false);
}

///////////////////////////////////// WIDGET ////////////////////////////////////////////////////////////////////
/*void UActorNarrationComponent::CreateNarrationWidget()
{
	//creates widget just like
	//sets created widget
	//WidgetComponent = /*Created widget#1#
}*/

void UActorNarrationComponent::InitWidgetComponent()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(this);
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

bool UActorNarrationComponent::SetWidgetVisibility(const bool bNewVisibility)
{
	
	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Widget component is invalid"));
		return false;
	}

	
	if (!bNewVisibility)
	{
		//We want to hide widget:
		//UE_LOG(LogTemp, Warning, TEXT("We try hide widget"))
		
		if (WidgetComponent->GetUserWidgetObject()) {WidgetComponent->GetUserWidgetObject()->RemoveFromParent();}
		if (WidgetComponent->GetUserWidgetObject()) {WidgetComponent->GetUserWidgetObject()->Destruct();}
		
		WidgetComponent->SetWidget(nullptr);
		return true;
	}

	//We want to show widget:
	//UE_LOG(LogTemp, Warning, TEXT("We Show widget"))

	//Get Widget Class:
	UClass* WidgetClassLoaded = nullptr;
	if(!WidgetClass.IsNull())
	{
		//Override has been specified therefore chose it:
		WidgetClassLoaded = WidgetClass.LoadSynchronous();
		if (!WidgetClassLoaded) { return false; }
	}
	else
	{
		//Override has not been specified therefore chose default:
		UActorNarrationController* ActorNarrationController = GetOwner()->GetGameInstance()->GetSubsystem<UActorNarrationController>();
		WidgetClassLoaded = ActorNarrationController->WidgetClass.LoadSynchronous();
		if (!WidgetClassLoaded)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load widget class"));
			return false;
		}
	}
	
	if(bNewVisibility)
	{
		//remove Current
		if (WidgetComponent->GetUserWidgetObject()) {WidgetComponent->GetUserWidgetObject()->RemoveFromParent();}
		if (WidgetComponent->GetUserWidgetObject()) {WidgetComponent->GetUserWidgetObject()->Destruct();}

		//Spawn new
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClassLoaded);
		if (!UserWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to CreateWidget"));
			return false;
		}
		
		//Spawned Succeeded therefore we initialize:
		WidgetComponent->SetWidget(UserWidget);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetDrawAtDesiredSize(true);
		return true;
	}
	//Spawn failed therefore we cleanup
	WidgetComponent->SetWidget(nullptr);
	return false;
}

bool UActorNarrationComponent::ValidateWidgetClass()
{
	UActorNarrationController* ActorNarrationController = GetOwner()->GetGameInstance()->GetSubsystem<UActorNarrationController>();
	if(ActorNarrationController->WidgetClass.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Widget class is null"));
		return false;
	}
	return true;
}

///////////////////////////////////// DT ACTIONS ///////////////////////////////////////////////////////////////
bool UActorNarrationComponent::GetNarrationLineDT(const TSoftObjectPtr<UDataTable>& MasterNarrationDT, TSoftObjectPtr<UDataTable>& OutDataTable, const FName& NarrationDTName)
{
	//Return false if name IsNone
	if (NarrationDTName.IsNone()) {	UE_LOG(LogTemp, Error,TEXT("NarrationDTName is none")); return false;	}
	
	//Load DT
	UDataTable* DataTableLoaded = MasterNarrationDT.LoadSynchronous();
	
	//Get DT row
	FMasterNarration* MasterNarration = DataTableLoaded->FindRow<FMasterNarration>(NarrationDTName, nullptr, false);

	//If row wasn't found then return false
	if (MasterNarration==nullptr) { UE_LOG(LogTemp, Error,TEXT("MasterNarrationDT is not set in component")); return false; }
	
	//Check found Row
	if(MasterNarration->DataTable.IsNull()) { UE_LOG(LogTemp, Error,TEXT("Row found in MasterNarrationDT is null")); return false;}
	
	//Return row
	OutDataTable = MasterNarration->DataTable;
	
	//Row has been found, return true
	return true;
}

bool UActorNarrationComponent::GetNarrationLineFromDT(const TSoftObjectPtr<UDataTable>& DataTable, FNarrationLine*& OutNarrationLine, const FName NarrationName)
{
	//Return false if name IsNone
	if (NarrationName.IsNone())
	{
		UE_LOG(LogTemp, Error,TEXT("NarrationName is invalid"));
		return false;
	}

	//Load DT
	UDataTable* DataTableLoaded = DataTable.LoadSynchronous();
	if (!DataTableLoaded)
	{
		UE_LOG(LogTemp, Error,TEXT("DataTable is invalid"));
		return false;
	}
	
	//Get DT row
	OutNarrationLine = DataTableLoaded->FindRow<FNarrationLine>(NarrationName, nullptr, false);

	//If row wasn't found then return false
	if (OutNarrationLine == nullptr)
	{
		UE_LOG(LogTemp, Error,TEXT("Could not find Narration line in DT")); return false;
	}

	//Row has been found, return true
	return true;
}

bool UActorNarrationComponent::GetTextAnimFromDT(const TSoftObjectPtr<UDataTable>& DataTable, const FName& PresetName, FNarrationDelays*& OutTextAnim)
{
	//Return false if name IsNone
	if (PresetName.IsNone())
	{
		UE_LOG(LogTemp, Error,TEXT("PresetName is none"));
		return false;
	}

	//Load DT
	UDataTable* DataTableLoaded = DataTable.LoadSynchronous();

	//Get DT row
	OutTextAnim = DataTableLoaded->FindRow<FNarrationDelays>(PresetName, nullptr, false);

	//If row wasn't found then return false
	if (OutTextAnim == nullptr)
	{
		UE_LOG(LogTemp, Error,TEXT("Could not find anim text preset in DT"));
		return false;
	}

	//Row has been found, return true
	return true;
}

///////////////////////////////////// TEXT Loop ///////////////////////////////////////////////////////////////

void UActorNarrationComponent::StartTextNarration(FNarrationLine NarrationLine)
{
	//When this narration line has no text just return
	if (NarrationLine.TextSequences.Num()<1)
	{
		//This doesnt mean that somebody forgot to type in text, this NarrationLine can be audio only by design.
		//So it's not necessary an error
		return;
	}

	//Try get NarrationDelay preset
	FNarrationDelays* TextAnimRow = nullptr;
	UActorNarrationController* ActorNarrationController = GetOwner()->GetGameInstance()->GetSubsystem<UActorNarrationController>();
	if (!GetTextAnimFromDT(ActorNarrationController->NarrationDelaysPreset, NarrationLine.DelaysPreset.GetTagName(),TextAnimRow))
	{
		UE_LOG(LogTemp,Error, TEXT("Could not find NarrationDelay preset"));
		return;
	}
	
	//If MasterDelayAtAlnumChar is <= 0. AND MasterDelayAtSpecialChar is <= 0
	//Then This Narration line is not animated. And will be displayed entirely at instant.
	if (TextAnimRow->MasterDelayAtAlnumChar <= 0 && TextAnimRow->MasterDelayAtSpecialChar <=0)
	{
		InstantDisplayTextNarration(NarrationLine.TextSequences);
	}
	else
	{
		//TODO Here I should initialize TimerHandle????
		
		NarrationDelays = *TextAnimRow;

		//Reset index
		TextLoopIndex = 0;

		//SetText sequences
		CurrentTextSequences = NarrationLine.TextSequences;
		//Starts the loop,
		TextLoop();
	}
}

void UActorNarrationComponent::StartTextLoop(FNarrationLine NarrationLine)
{
	//TODO Delet this, it got refactored
}

void UActorNarrationComponent::InstantDisplayTextNarration(const TArray<FTextSequence> TextSequences)
{
	FString LastChar;
	FString RichFormatNarration = GetNarrationTextAtIndex(GetNarrationLen(TextSequences)-1, TextSequences, LastChar);

	//WidgetComponent		 interface call This 
	INarrationWidgetActions::Execute_UpdateNarration(WidgetComponent->GetWidget(),RichFormatNarration);
	NarrationElementFinished(TimerHandleText);
}

void UActorNarrationComponent::TextLoop()
{
	//Last char at current index
	FString LastChar;
	//Narration Text to be sent to the widget 
	FString RichFormatNarration = GetNarrationTextAtIndex(TextLoopIndex, CurrentTextSequences, LastChar);
	
	//End Loop condition
	// || WidgetComponent->GetWidget() == nullptr
	if (RichFormatNarration.IsEmpty())
	{
		NarrationElementFinished(TimerHandleText);
		return;
	}
	
	//WidgetComponent		 interface call This 
	INarrationWidgetActions::Execute_UpdateNarration(WidgetComponent->GetWidget(),RichFormatNarration);

	//TextLoopIndex++;
	TextLoopIndex = TextLoopIndex + 1;
	
	//Get Delay for current char
	float Delay = GetDelayForChar( LastChar, NarrationDelays );
	
	//Call itself with delay  
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandleText, this, &UActorNarrationComponent::TextLoop, Delay, false);
}

float UActorNarrationComponent::GetDelayForChar(FString Char, const FNarrationDelays& TextAnimRow)
{
	//Delay used when the value of delay is below 0 
	float MinimalDelay = 0.0001f;
	
	if (FChar::IsAlnum(Char[0]))
	{
		return TextAnimRow.MasterDelayAtAlnumChar > 0 ? TextAnimRow.MasterDelayAtAlnumChar : MinimalDelay;
	}
	
	switch (Char[0])
	{
	case TEXT(' '):
		if (TextAnimRow.DelayAtSpace == -1 )
		{
			return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
		}
		return TextAnimRow.DelayAtSpace > 0 ? TextAnimRow.DelayAtSpace: MinimalDelay;
	case TEXT('?'):
		if (TextAnimRow.DelayAtQuestionMark == -1 )
		{
			return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
		}
		return TextAnimRow.DelayAtQuestionMark > 0 ? TextAnimRow.DelayAtQuestionMark : MinimalDelay;
	case TEXT('!'):
		if (TextAnimRow.DelayAtExclamationMark == -1 )
		{
			return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
		}
		return TextAnimRow.DelayAtExclamationMark > 0 ? TextAnimRow.DelayAtExclamationMark : MinimalDelay;
	case TEXT(','):
		if (TextAnimRow.DelayAtComa == -1 )
		{
			return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
		}
		return TextAnimRow.DelayAtComa > 0 ? TextAnimRow.DelayAtComa : MinimalDelay;
	case TEXT('.'):
		if (TextAnimRow.DelayAtDot == -1 )
		{
			return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
		}
		return TextAnimRow.DelayAtDot > 0 ? TextAnimRow.DelayAtDot : MinimalDelay;
	}

	return TextAnimRow.MasterDelayAtSpecialChar > 0 ? TextAnimRow.MasterDelayAtSpecialChar : MinimalDelay;
	
	/*if (Char[0] == '?')
	{
		return TextAnimRow.DelayAtQuestionMark;
	}
	else if (Char[0] == '!')
	{
		return TextAnimRow.DelayAtExclamationMark;
	}
	else if (Char[0] == ',')
	{
		return TextAnimRow.DelayAtComa;
	}
	else if (Char[0] == '.')
	{
		return TextAnimRow.DelayAtDot;
	}
	else if (FChar::IsPunct(Char[0]))
	{
		return TextAnimRow.MasterDelayAtSpecialChar;
	}
	else
	{
		return TextAnimRow.SecondsPerCharacter;
	}*/
}

FString UActorNarrationComponent::GetNarrationTextAtIndex(const int32& Index, const TArray<FTextSequence> TextSequences, FString& LastChar)
{
	int32 CurrentLen = 0;
	FString NarrationFullLine = FString();
	for (auto& Element : TextSequences)
	{
		FName CurrentStyle = Element.TextStylePreset.GetTagName();
		FString CurrentText = Element.Text.ToString();

		//set current length
		CurrentLen = CurrentLen + CurrentText.Len();

		//Get text for this loop
		CurrentText = CurrentText.LeftChop((CurrentLen-1-Index));
		//Get last char
		LastChar = CurrentText.RightChop(CurrentText.Len()-1);

		//Append rich text formatting 
		FString Temp = FString();
		Temp += "<";
		Temp += CurrentStyle.ToString();
		Temp += ">";
		Temp += CurrentText;
		Temp += "</>";

		//Append to Full
		NarrationFullLine += Temp;

		if (CurrentLen-1>=Index)
		{
			return NarrationFullLine;
		}
	}
	
	//We ran out of characters for this narration therefore return empty
	LastChar = FString();
	return FString();
}

int32 UActorNarrationComponent::GetNarrationLen(const TArray<FTextSequence> TextSequences)
{
	int32 Len = 0;
	for (auto Element : TextSequences)
	{
		Len += Element.Text.ToString().Len();
	}
	return Len;
}

///////////////////////////////////// AUDIO Loop ///////////////////////////////////////////////////////////////
void UActorNarrationComponent::StartAudioNarration(const TSoftObjectPtr<USoundBase>& Audio, bool bLoop)
{
	
}

