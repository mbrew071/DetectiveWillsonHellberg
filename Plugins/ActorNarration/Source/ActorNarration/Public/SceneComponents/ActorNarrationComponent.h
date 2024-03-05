// All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Structures/NarrationLine.h"
#include "Structures/NarrationDelays.h"
#include "Blueprint/UserWidget.h"
#include "ActorNarrationComponent.generated.h"

class UWidgetComponent;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNarrationEnd, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNarrationDisrupt, FName ,NarrationName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hideCategories = (Rendering, Replication, Input, Actor, Base, Collision, Shape, Transform, Sockets), hidecategories=Physics, hidecategories=Sockets)
class ACTORNARRATION_API UActorNarrationComponent : public USceneComponent
{
	GENERATED_BODY()

	/////////////////////////////////////// GENERAL ///////////////////////////////////////////////////////
public:	
	// Sets default values for this component's properties
	UActorNarrationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	///////////////////////////////////// WIDGET /////////////////////////////////////////////////////////////
public:
	//Component that stores the dynamically spawned widget
	UPROPERTY()
	UWidgetComponent* WidgetComponent;
	
	//IF this class is none then the default widget will be used ( set in Controller subsystem)
	//Use this when I want the character to have a custom text displaying widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;

private:
	void InitWidgetComponent();

	//Shows or hides the widget that indicates that Character is close enough to interact
	UFUNCTION(BlueprintCallable, Category="Design|Widget")
	bool SetWidgetVisibility (const bool bNewVisibility);
	
	///////////////////////////////////// Audio   /////////////////////////////////////////////
public:
	//When Audio narration is ongoing this component will be playing it
	UPROPERTY()
	UAudioComponent* AudioComponent;
	
	///////////////////////////////////// Narration /////////////////////////////////////////////////////////
private:	
	//Narration is currently being played if this is other than "None" OR other than invalid. 
	UPROPERTY()
	FName OngoingNarration = NAME_None;
public:
	//Returns true if a narration is currently being played
	UFUNCTION(BlueprintCallable, Category="Narration")
	bool IsNarrationOngoing();
	
public:
	/**
	* Starts playing a Narration Line. This Narration line must be inside a DT that is stored by MasterNarrationDT.
	* Returns false if it failed to start playing a new narration. (eg. narration was not found in DT, something was set up incorrectly.
	* Returns true if it successfully started playing a narration  
	*
	* If other narration is currently ongoing it will be replaced with this new narration.
	*
	* OnNarrationDisrupt is called when ongoing narration is replaced by another.
	* OnNarrationBegin is called when Narration begins 
	* OnNarrationEnd is called when Narration ends 
	*
	* @param DTName	Name of DT with NarrationLines, this DTis stored in MasterNarrationDT. 
	* @param NarrationLineName Name of NarrationLine that it will play
	*/
	UFUNCTION(BlueprintCallable, Category="Narration")
	bool PlayNarrationMasterDT(FName DTName = NAME_None, FName NarrationLineName = NAME_None); 
	
	/**
	* Starts playing Narration Line. If other narration is currently ongoing it will be replaced with this new one.
	* Returns false if it failed to start playing a new narration. (eg. narration was not found in DT, something was set up incorrectly.  
	* Returns true if it successfully started playing a narration
	* 
	* OnNarrationDisrupt is called when ongoing narration is replaced by another.
	* OnNarrationBegin is called when Narration begins 
	* OnNarrationEnd is called when Narration ends 
	*
	* @param DataTable	Data table that contains narration to be played
	* @param NarrationLineName	Must not be "None"
	*/
	UFUNCTION(BlueprintCallable, Category="Narration")
	bool PlayNarration(const TSoftObjectPtr<UDataTable>& DataTable, FName NarrationLineName = NAME_None); 

	/**
	* Useful for dynamically generated narrations OR narrations that arent in the default DTs 
	* Starts playing Narration Line from struct. If other narration is currently ongoing it will be replaced with this new one.
	* Returns false if it failed to start playing a new narration. (eg. something was missing in struct etc.  
	* Returns true if it successfully started playing a narration
	* 
	* OnNarrationDisrupt is called when ongoing narration is replaced by another.
	* OnNarrationBegin is called when Narration begins 
	* OnNarrationEnd is called when Narration ends 
	*
	* @param NarrationLine	Struct that contains narration to be played
	* @param NarrationLineName	Must not be "None"
	*/
	UFUNCTION(BlueprintCallable, Category="Narration")
	bool PlayNarrationStruct(const FNarrationLine& NarrationLine, FName NarrationLineName = NAME_None);
	
private:
	/*Main function for playing narration line*/
	bool HandleNarrationProcess(const FNarrationLine* NarrationLine, FName NarrationLineName);
	
	bool ValidateWidgetClass();

	////////////////// Narration DT utility functions:
private:
	/*Searches a DT that contains DT with Narration lines (FMasterNarration)*/
	static bool GetNarrationLineDT(const TSoftObjectPtr<UDataTable>& MasterNarrationDT, TSoftObjectPtr<UDataTable>& OutDataTable, const FName& NarrationDTName = NAME_None);

	/*Searches a DT that contains Narration lines (FMasterNarration)*/
	static bool GetNarrationLineFromDT(const TSoftObjectPtr<UDataTable>& DataTable, FNarrationLine*& OutNarrationLine, const FName NarrationName = NAME_None);
	
	/*Searches a DT that contains TextAnim presets (FTextAnim)*/
	static bool GetTextAnimFromDT(const TSoftObjectPtr<UDataTable>& DataTable, const FName& PresetName,  FNarrationDelays*& OutTextAnim);
	
	////////////////////// Narration Actions:

private:
	//Called when Narration begins
	//It initializes variables for the new narration to begin
	void InitBeginNarration(FName NarrationName, const ENarrationEndRule& NarrationEndRule);
	
	//Called when Narration stops
	// bSuccess is true when whole narration was played successfully. (Which means played fully OR explicitly skipped)
	// bSuccess is false when narration was disrupted and therefore couldn't be played fully.  
	void EndNarration(bool bSuccess);

public:
	//It stops currently played narration line.
	//When this function finishes the OnNarrationEnd (bSuccess false) will be called
	UFUNCTION(BlueprintCallable, Category="Narration")
	void DisruptNarration();

	//It skips currently played narration line.
	//When this function finishes the OnNarrationEnd (bSuccess true) will be called
	UFUNCTION(BlueprintCallable, Category="Narration")
	void SkipNarration();
	
	///////////////////////////////////// Delegates ///////////////////////////////////////////////////////////////
public:
	//Called when narration begins
	UPROPERTY(BlueprintAssignable, Category="Binds")
	FOnNarrationBegin OnNarrationBegin;

	//Called when narration ends
	//True when narration wasn't disrupted
	//False when narration ended because it was disrupted and therefore wasn't played fully.
	UPROPERTY(BlueprintAssignable, Category="Binds")
	FOnNarrationEnd OnNarrationEnd;

private:
	//This is called when one part of narration has finished eg. Text loop or Audio Loop
	void NarrationElementFinished(FTimerHandle& TimerHandle);

	//Calls the end of narration after the Outro delay time passed.
	void EndNarrationWithOutroDelay();

	///////////////////////////////////// Text Loop ///////////////////////////////////////////////////////////
	
	//Handle for Text loop
	UPROPERTY()
	FTimerHandle TimerHandleText;

	//Index of current char for the current Narration
	UPROPERTY()
	uint32 TextLoopIndex = 0;

	//Array of text sequence for the current Text loop
	UPROPERTY()
	TArray<FTextSequence> CurrentTextSequences;

	//Animation preset fot the current Text Loop
	UPROPERTY()
	FNarrationDelays NarrationDelays;
	
	//End rule for the current Text loop
	UPROPERTY()
	TEnumAsByte<ENarrationEndRule> CurrentNarrationEndRule;

	//Starts playing text narration. Either Instantly shows full narration or starts a TextLoop
	void StartTextNarration(FNarrationLine NarrationLine);
	
	//Starts the loop that displays the text narration
	void StartTextLoop(FNarrationLine NarrationLine);

	//Plays the whole Text narration instantly, without animation.
	void InstantDisplayTextNarration(const TArray<FTextSequence> TextSequences);
	
	//This function cannot have args. 
	UFUNCTION()
	void TextLoop();
	
	//returns delay for provided char. Delay is based on Delay preset.
	float GetDelayForChar(FString Char, const FNarrationDelays& TextAnimRow);

	//returns string to be displayed in Widget
	FString GetNarrationTextAtIndex(const int32& Index, const TArray<FTextSequence> TextSequences, FString& LastChar);

	int32 GetNarrationLen(const TArray<FTextSequence> TextSequences);
	
	///////////////////////////////////// AUDIO Loop //////////////////////////////////////////////////////////

	//Handle for Audio loop
	UPROPERTY()
	FTimerHandle TimerHandleAudio;
	
	//Start playing Audio narration. It will loop audio only when End rule is OnTextWhileAudioLooped.
	void StartAudioNarration(const TSoftObjectPtr<USoundBase>& Audio, bool bLoop = false);
	
};
