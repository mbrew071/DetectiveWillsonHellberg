// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelChangeManager.generated.h"

/**
 * This subsystem is responsible for transition between levels
 * It works only in standalone or build. In viewport it will change level only one time. 
 *
 * Transition process starts by calling StartWorldTransition();
 * Process goes as follows:
 * 1. Transitions to the TransitionWorld that is defined in project settings
 * 2. Creates and adds a widget if its class was defined during plugin initialization
 * 3. Async loads the TargetWorld - a world to which game will transition
 * 4. When TargetLevel is loaded it either opens that level automatically OR it waits and opens it when TriggerTransition(); gets called   
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetWorldOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetWorldLoaded,bool, AutoTransition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChangeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTransitionWorldOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerTransitionReady);

UCLASS()
class LEVELCHANGER_API ULevelChangeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()


////Main functions:	
public:
	/** 
	 * @param InTransitionWidget Class of widget that will be created during the loading of target level 
	 * @return True when initialization is a success, and plugin has all data that it requires
	 */
	UFUNCTION(BlueprintCallable, Category="LevelChanger")
	bool InitializePlugin(TSoftClassPtr<UUserWidget> InTransitionWidget);
	
	/**
	 * This function Starts a process of level transition, It will go to transition map 
	 * @param InTargetWorld World to which we will transition
	 * @param bInAutoTransition When true after target level gets loaded it will transition to it automatically. When false it will transition to the loaded level when TriggerTransition() is called
	 * @param InAutoTransitionDelay Once TargetLevel gets loaded it will wait for this amount of time until TriggerTransition is auto called. This does nothing if bAutoTransition is false.   
	 *
	 * @return If false the Level ref was invalid OR Transition map in project settings was not set. 
	 */
	UFUNCTION(BlueprintCallable, Category="LevelChanger")
	void StartWorldTransition(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay); 

//// Transition World:
private:
	/**
	 * @param  OutWorld TransitionLevel defined in project settings.
	 * @return True when transition world was defined in project settings
	 */
	UFUNCTION(BlueprintCallable, Category="LevelChanger")
	bool GetTransitionWorld(TSoftObjectPtr<UWorld>& OutWorld);

	//This initializes the async loading of level
	bool OpenTransitionWorld();
	void OnTransitionWorldCreation(UWorld* World);
	void OnTransitionWorldBeginPlay();

	//TransitionVariables
	UPROPERTY()
	bool bIsTransitionOngoing;
	UPROPERTY()
	TSoftObjectPtr<UObject> SoftTargetWorld;
	UPROPERTY()
	bool bAutoTransition;
	UPROPERTY()
	float AutoTransitionDelay;

	//// TargetWorld: 
private:

	//This variable stores the TargetLevel into which we will transition
	UPROPERTY()
	UWorld* TargetWorld;

	//This is to call LoadTargetWorld but it does it with a 1s delay
	void LoadWithFrameDelay(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay);
	
	//LoadsPackage of the TargetWorld
	//When loading finishes executes OnLevelAsyncLoadedAutoTrigger OR OnLevelAsyncLoadedNoAutoTrigger
	void LoadTargetWorld(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay);
	
	//Called after level got loaded, it will broadcast that it waits for signal to transition
	void OnLevelAsyncLoadedNoAutoTrigger(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);

	//Called after level gets loaded, it will automatically trigger transition
	void OnLevelAsyncLoadedAutoTrigger(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result, float InAutoTransitionDelay);

	//This will Trigger Transition from TransitionMap to the loaded TargetLevel
	UFUNCTION(BlueprintCallable,Category="LevelChanger")
	bool TriggerTransition();
	void OnTargetLevelOpened(UWorld* World);

	//// Widget:
private:
	//Class of the widget which will be while game is in TransitionWorld
	UPROPERTY(EditAnywhere, Category="LevelChanger")
	TSoftClassPtr<UUserWidget> TransitionWidgetClass;

	/*
	UPROPERTY(EditDefaultsOnly, Category="LevelChanger")
	UUserWidget* TransitionWidget;
	*/

	//Creates and adds to viewport a transition widget
	bool CreateTransitionWidget(TSoftClassPtr<UUserWidget> WidgetClass);
	
//// Utility:
private:
	//Converts result of async load into a world
	bool GetWorldFromPackage(const FName& PackageName, UPackage* LoadedPackage, UWorld*& OutWorld);
	
	//Converts TSoftObjectPtr to Package that is required by LoadPackageAsync()
	bool TSoftObjectPtrToPackage(const TSoftObjectPtr<UObject>& World, FString& OutPackage);

//// Delegates:
public:

	//Called when level transition begins, Before the TransitionWorld is Opened 
	UPROPERTY(BlueprintAssignable, Category="LevelChanger", SaveGame)
	FOnLevelChangeBegin OnLevelChangeBegin;

	//Called after TransitionWorld is opened 
	UPROPERTY(BlueprintAssignable, Category="LevelChanger")
	FOnTransitionWorldOpened OnTransitionLevelOpened;

	//Called after TargetWorldOpened is opened.
	UPROPERTY(BlueprintAssignable, Category="LevelChanger")
	FOnTargetWorldOpened OnTargetWorldOpened;
	
	//Called when the async level load finished and player can trigger level transition
	UPROPERTY(BlueprintAssignable, Category="LevelChanger")
	FOnTriggerTransitionReady OnTriggerTransitionReady;

	//Called when async loading of target level finished
	UPROPERTY(BlueprintAssignable, Category="LevelChanger")
	FOnTargetWorldLoaded OnTargetWorldLoaded;
	
	UFUNCTION(BlueprintCallable, Category="LevelChanger")
	bool IsBound();
};
