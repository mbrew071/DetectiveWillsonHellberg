// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Enumerations/SimpleNarrationSkipRule.h"
#include "Interfaces/NarrationGraph_I.h"
#include "LatentActions.h"
#include "Engine/LatentActionManager.h"
#include "Structures/NarrationOption.h"
#include "Structures/ScreenNarrationPiece.h"
#include "Structures/SceneNarrationPiece.h"
#include "GraphNarrationDelegates.h"
#include "Blueprint/UserWidget.h"
#include "NarrationGraph.generated.h"

UENUM(BlueprintType)
enum class ENarrationPinType : uint8
{
	Completed,
	Failed
};

//////////////// Latent ActionPlayNarration ////////////////////////////////////////////////
class FPlayNarrationLatentAction : public FPendingLatentAction
{
public:
	//My Variables:
	ENarrationPinType& OutcomeRef;
	FGameplayTag& FailureReasonRef;
	//LatentAction variables:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsFinished;

	FPlayNarrationLatentAction(ENarrationPinType& InOutcome, FGameplayTag& InFailureReason,
		const FLatentActionInfo& LatentInfo)
		//My Variables:
		: OutcomeRef(InOutcome)
		, FailureReasonRef(InFailureReason)
		//LatentAction variables:
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, bIsFinished(false)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(bIsFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}

	void FinishAction(ENarrationPinType Outcome, const FGameplayTag& FailureReason = FGameplayTag())
	{
		//My Variables:
		OutcomeRef = Outcome;
		FailureReasonRef = FailureReason;
		//LatentAction variables:
		bIsFinished = true;
	}
};

class FPlaySceneNarrationLatentAction : public FPendingLatentAction
{
public:
	//My Variables:
	ENarrationPinType& OutcomeRef;
	FGameplayTag& FailureReasonRef;
	//LatentAction variables:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsFinished;

	FPlaySceneNarrationLatentAction(ENarrationPinType& InOutcome, FGameplayTag& InFailureReason,
		const FLatentActionInfo& LatentInfo)
		//My Variables:
		: OutcomeRef(InOutcome)
		, FailureReasonRef(InFailureReason)
		//LatentAction variables:
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, bIsFinished(false)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(bIsFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}

	void FinishAction(ENarrationPinType Outcome, const FGameplayTag& FailureReason = FGameplayTag())
	{
		//My Variables:
		OutcomeRef = Outcome;
		FailureReasonRef = FailureReason;
		//LatentAction variables:
		bIsFinished = true;
	}
};

class FDisplayOptionsLatentAction : public FPendingLatentAction
{
public:
	//My Variables:
	ENarrationPinType& OutcomeRef;
	FGameplayTag& FailureReasonRef;
	int32& ChosenOptionRef;
	//LatentAction variables:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsFinished;

	FDisplayOptionsLatentAction(ENarrationPinType& InOutcome, FGameplayTag& InFailureReason, int32& ChosenOption, const FLatentActionInfo& LatentInfo)
		//My Variables:
		: OutcomeRef(InOutcome)
		, FailureReasonRef(InFailureReason)
		, ChosenOptionRef(ChosenOption)
		//LatentAction variables:
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, bIsFinished(false)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(bIsFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}

	void FinishAction(ENarrationPinType Outcome, const FGameplayTag& FailureReason = FGameplayTag(), const int32& ChosenOption = 0)
	{
		//My Variables:
		OutcomeRef = Outcome;
		FailureReasonRef = FailureReason;
		ChosenOptionRef = ChosenOption;
		//LatentAction variables:
		bIsFinished = true;
	}
};

class FActionCustomLatentAction : public FPendingLatentAction
{
public:
	//My Variables:
	ENarrationPinType& OutcomeRef;
	FGameplayTag& FailureReasonRef;
	const UObject* DTORef;
	//LatentAction variables:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsFinished;

	FActionCustomLatentAction(ENarrationPinType& InOutcome, FGameplayTag& InFailureReason, const UObject* DTO, const FLatentActionInfo& LatentInfo)
		//My Variables:
		: OutcomeRef(InOutcome)
		, FailureReasonRef(InFailureReason)
		, DTORef(DTO)
		//LatentAction variables:
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
		, bIsFinished(false)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		Response.FinishAndTriggerIf(bIsFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}

	void FinishAction(ENarrationPinType Outcome, const FGameplayTag& FailureReason = FGameplayTag(), const UObject* DTO = nullptr )
	{
		//My Variables:
		OutcomeRef = Outcome;
		FailureReasonRef = FailureReason;
		DTORef = DTO;
		//LatentAction variables:
		bIsFinished = true;
	}
};

/**
 * Narration graph class
 */
UCLASS(Blueprintable)
class SIMPLENARRATION_API UNarrationGraph : public UObject, public INarrationGraph_I
{
	GENERATED_BODY()
	
	//Used to prevent narration action from starting when this graph is cancelled
	//This is for situation When this graph has been cancelled AND in graph delay node is ongoing after which PlaySceneNarration will be triggered. this bool will prevent PlaySceneNarration from playing.
	UPROPERTY()
	bool bActive = true;

	//If I create this override then derived BP classes will be able to call static functions that require WorldContext eg. "GetGameInstance" OR any function from the BPFL   
	virtual UWorld* GetWorld() const override;

	//////////////////// Graph member variables ////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GraphNarration")
	TEnumAsByte<ESimpleNarrationSkipRule> DefaultSkipRule;

	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	TArray<AActor*> NarrationActors;
	
	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	UUserWidget* ScreenWidget;

	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	UUserWidget* SceneWidget;

	//////////////////// Graph Events ////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="GraphNarration")
	void BeginGraph();
	
	//////////////////// API for other classes ////////////////////////////////////////////////
	
	//This function cancels currently ongoing narration
	//It will cause "Fail" pin to trigger
	//This would be called when eg. player goes into combat during dialogue
	UFUNCTION(BlueprintCallable, Category="GraphNarration")
	void FailAllActions(const FGameplayTag Reason);
	
	/////////////////////////////// ACTIONS /////////////////////////////////////////////////////
public:
	
	// Ends dialogue graph
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Action", meta=(HideSelfPin="true", CompactNodeTitle="End Graph", AdvancedDisplay="Reason"))
	void ActionEndGraph(const FGameplayTag Reason);

	//Rename to viewportNarration? ScreenNarration?
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Action", Meta=(Latent, LatentInfo="LatentInfo", ExpandEnumAsExecs = "Outcome", HideSelfPin="true", AdvancedDisplay="FailureReason"))
	void ActionPlayScreenNarration(ENarrationPinType& Outcome, const TArray<FScreenNarrationPiece> Phrases, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Action", Meta=(Latent, LatentInfo="LatentInfo", ExpandEnumAsExecs = "Outcome", HideSelfPin="true", AdvancedDisplay="FailureReason"))
	void ActionPlaySceneNarration(ENarrationPinType& Outcome, UPARAM(ref) AActor*& Actor, const TArray<FSceneNarrationPiece> Phrases, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo);

	
	/**
	 * This function displays Text
	 * @param Options List of dialogue options to display
	 * @param DecisionTime TODO add type or refactor this to some generic enum array for additional params
	 * @param ChosenOption index of option that player chose.
	 * @param FailureReason reason why this action failed. This is only valid if "Failed" pin executed.
	 */
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Action", meta=(Latent, LatentInfo="LatentInfo", ExpandEnumAsExecs = "Outcome", HideSelfPin="true", AdvancedDisplay="DecisionTime, FailureReason"))
	void ActionDisplayOptions(ENarrationPinType& Outcome,const TArray<FNarrationOption> Options, int32& ChosenOption, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo,  const float DecisionTime = -1.0f );
	/**
	 * This is custom scriptable action
	 * @param ActionTag Tag of the custom action to execute
	 * @param DTO Data transfer objects. If you want to transfer custom data to the action then use this.
	 * @param FailureReason If execution fails this tag will provide reason why. 
	 */
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Action", meta=(Latent, LatentInfo="LatentInfo", ExpandEnumAsExecs = "Outcome", AdvancedDisplay="DTO, FailureReason", HideSelfPin="true"))
	void ActionCustom(ENarrationPinType& Outcome, const FGameplayTag ActionTag, const UObject* DTO, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo);
	
	//Interface functions
	virtual void PlayScreenNarrationCompleted_Implementation() override;
	virtual void PlayScreenNarrationFailed_Implementation(const FGameplayTag& FailureReason) override;
	virtual void PlaySceneNarrationCompleted_Implementation() override;
	virtual void PlaySceneNarrationFailed_Implementation(const FGameplayTag& FailureReason) override;
	virtual void DisplayOptionsCompleted_Implementation(const int32& ChosenOptions) override;
	virtual void DisplayOptionsFailed_Implementation(const FGameplayTag& FailureReason) override;
	virtual void ActionCustomCompleted_Implementation() override;
	virtual void ActionCustomFailed_Implementation(const FGameplayTag& FailureReason) override;

	/////////////////////////////// Actor narration variables ////////////////////////////////////////
	
protected:
	/// Booleans
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta = (HideSelfPin = "true", Keywords="SetBoolVariable"))
	void SetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, const bool Value);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound, Scope", HideSelfPin = "true", Keywords="GetBoolVariable"))
	void GetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, bool& Value, const bool bCreateIfNotFound = false);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound", HideSelfPin = "true", Keywords="GetBoolVariable"),BlueprintPure, DisplayName="Get Narration Bool")
	void GetNarrationBool_Pure(UPARAM(ref) AActor* Actor, const FName VariableName, const FGameplayTag Scope,  bool& bSuccess, bool& Value, const bool bCreateIfNotFound = false);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin = "true", Keywords="ClearBool, RemoveBoolVariable"))
	void RemoveNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName); 
	
	/// Int
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin = "true", Keywords="SetIntVariable"))
	void SetNarrationInt(UPARAM(ref) AActor* Actor, const FName Name,  const int Value);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound", HideSelfPin="true", Keywords="GetIntVariable"))
	void GetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound = false);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound", HideSelfPin="true", Keywords="GetIntVariable"), BlueprintPure, DisplayName="Get Narration Int")
	void GetNarrationInt_Pure(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound = false);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin="true", Keywords="ClearInt, RemoveIntVariable"))
	void RemoveNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName); 

	/////////////////////////////// Global narration variables ////////////////////////////////////////
	
protected:
	/// Booleans
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta = (HideSelfPin = "true", Keywords="SetBoolVariable", AdvancedDisplay="Scope"))
	void SetNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope,  const bool Value);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound, Scope", HideSelfPin = "true", Keywords="GetBoolVariable"))
	void GetNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, bool& Value, const bool bCreateIfNotFound = false);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound, Scope", HideSelfPin = "true", Keywords="GetBoolVariable"),BlueprintPure, DisplayName="Get Narration Bool Global")
	void GetNarrationBoolGlobal_Pure(const FName VariableName, const FGameplayTag Scope,  bool& bSuccess, bool& Value, const bool bCreateIfNotFound = false);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin = "true", Keywords="ClearBool, RemoveBoolVariable", AdvancedDisplay="Scope"))
	void RemoveNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope); 
	
	/// Int
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin = "true", Keywords="SetIntVariable",AdvancedDisplay="Scope"))
	void SetNarrationIntGlobal(const FName Name, const FGameplayTag Scope, const int Value);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound, Scope", HideSelfPin="true", Keywords="GetIntVariable"))
	void GetNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound = false);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(AdvancedDisplay="bCreateIfNotFound, Scope", HideSelfPin="true", Keywords="GetIntVariable"), BlueprintPure, DisplayName="Get Narration Int Global")
	void GetNarrationIntGlobal_Pure(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound = false);
	
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Variable", meta=(HideSelfPin="true", Keywords="ClearInt, RemoveIntVariable", AdvancedDisplay="Scope"))
	void RemoveNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope);
	
	///////////////////////////// GetNarrationActorOfClass
protected:
	/**
	* @param Class to search
	* @param SearchedNumber Determines which actor that matches tag will be returned.
	* if 0 and array has one actor that matches tag this actor will be returned.
	* if 1 and array has two actors that match tag the second actor will be returned
	* if 1 and array has one actor, a nullptr will be returned
	* @param Actor actor that matches @param Class AND @param SearchedNumber 
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor", AdvancedDisplay="SearchedNumber"))
	void GetNarrationActorOfClass(bool& bSuccess, const TSoftClassPtr<AActor> Class, AActor*& Actor, const int32 SearchedNumber);

	/**
	* @param Class to search
	* @param SearchedNumber Determines which actor that matches tag will be returned.
	* if 0 and array has one actor that matches tag this actor will be returned.
	* if 1 and array has two actors that match tag the second actor will be returned
	* if 1 and array has one actor, a nullptr will be returned
	* @param Actor actor that matches @param Class AND @param SearchedNumber 
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor"), BlueprintPure, DisplayName="Get Narration Actor Of Class")
	void GetNarrationActorOfClass_Pure(bool& bSuccess, const TSoftClassPtr<AActor> Class, AActor*& Actor, const int32 SearchedNumber = 0);

	/**
	* @param Index to get
	* @param Actor actor under @param Index
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor"), DisplayName="Get Narration Actor")
	void GetNarrationActor(bool& bSuccess, const int32 Index, AActor*& Actor);

	/**
	* @param Index to get
	* @param Actor actor under @param Index
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor"), BlueprintPure, DisplayName="Get Narration Actor")
	void GetNarrationActor_Pure(bool& bSuccess, const int32 Index, AActor*& Actor);

	/**
	 * @param Tag Tag to search
	 * @param SearchedNumber Determines which actor that matches tag will be returned.
	 * if 0 and array has one actor that matches tag this actor will be returned.
	 * if 1 and array has two actors that match tag the second actor will be returned
	 * if 1 and array has one actor, a nullptr will be returned
	 * @param Actor actor that matches @param Tag AND @param SearchedNumber 
	 * @return true if found. false if not found
	 */
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor", AdvancedDisplay="SearchedNumber"))
	void GetNarrationActorWithTag(bool& bsuccess, const FName Tag, AActor*& Actor, const int32 SearchedNumber = 0);
	
	/**
	* @param Tag Tag to search
	* @param SearchedNumber Determines which actor that matches tag will be returned.
	* if 0 and array has one actor that matches tag this actor will be returned.
	* if 1 and array has two actors that match tag the second actor will be returned
	* if 1 and array has one actor, a nullptr will be returned
	* @param Actor actor that matches @param Tag AND @param SearchedNumber 
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor", AdvancedDisplay="SearchedNumber"), BlueprintPure, DisplayName="Get Narration Actor With Tag")
	void GetNarrationActorWithTag_Pure(bool& bSuccess, const FName Tag, AActor*& Actor, const int32 SearchedNumber = 0);

	/**
	* @param Class to search
	* @param SearchedNumber Determines which actor that matches tag will be returned.
	* if 0 and array has one actor that matches tag this actor will be returned.
	* if 1 and array has two actors that match tag the second actor will be returned
	* if 1 and array has one actor, a nullptr will be returned
	* @param Actor actor that matches @param Class AND @param SearchedNumber 
	* @return true if found. false if not found
	*/
	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor", AdvancedDisplay="SearchedNumber"))
	void GetNarrationActorWithInterface(bool& bSuccess, TSubclassOf<UInterface> Interface, AActor*& Actor, const int32 SearchedNumber);

	UFUNCTION(BlueprintCallable, Category="GraphNarration|Actor", meta=(HideSelfPin="true", Keywords="GetNPC, GetActor", AdvancedDisplay="SearchedNumber"),BlueprintPure, DisplayName="Get Narration Actor With Interface")
	void GetNarrationActorWithInterface_Pure(bool& bSuccess, TSubclassOf<UInterface> Interface, AActor*& Actor, const int32 SearchedNumber);

	////////////////// Latent action 
private:
	FPlayNarrationLatentAction* CurrentPlayNarrationAction;
	FDisplayOptionsLatentAction* CurrentDisplayOptionsAction;
	FActionCustomLatentAction* CurrentActionCustom;
	FPlaySceneNarrationLatentAction* CurrentSceneNarrationAction;


/////////// Create widgets
	
	bool CreateScreenNarrationWidget(UUserWidget*& OutWidget);
	bool CreateSceneNarrationWidget(UUserWidget*& OutWidget);


	////////////////////
public:
	FOnGraphNarrationEnd OnGraphNarrationEnd;

};
