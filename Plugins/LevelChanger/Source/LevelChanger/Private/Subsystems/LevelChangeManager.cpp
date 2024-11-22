// All rights reserved KAG 2024


#include "Subsystems/LevelChangeManager.h"

#include "GameMapsSettings.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "UObject/Package.h"
#include "UObject/UObjectIterator.h"

#include "Engine/AssetManager.h"

#include "Kismet/GameplayStatics.h"

////Main functions:

bool ULevelChangeManager::InitializePlugin(TSoftClassPtr<UUserWidget> InTransitionWidget)
{
	if (InTransitionWidget.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("InTransitionWidget is invalid"))
		return false;
	}
	TransitionWidgetClass = InTransitionWidget; 
	return true;
}

void ULevelChangeManager::OpenLevel(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay, const FString InOptions)
{
	if(bIsTransitionOngoing)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot start another transition before previous one ends"));
		return;
	}
	
	bIsTransitionOngoing = true;
	SoftTargetWorld = InTargetWorld;
	bAutoTransition = bInAutoTransition;
	AutoTransitionDelay = InAutoTransitionDelay;
	Options = InOptions;
	
	if (InTargetWorld.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("InTargetWorld is not valid"));
		return;
	}
	
	//Go to transition level
	if (!OpenTransitionWorld()) { return; }
}

///// Transition World:

bool ULevelChangeManager::OpenTransitionWorld()
{
	TSoftObjectPtr<UWorld> TransitionWorld;
	if (!GetTransitionWorld(TransitionWorld))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find transition level"))
		return false;
	}
	if (TransitionWorld.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Transition world is not set in project settings"))
		return false;
	}
	
	OnLevelChangeBegin.Broadcast();
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull), TransitionWorld);

	//OnPostWorldInitialization Too early to GetPlayerController inside widget so I cannot do Focus()
	//OnPostWorldCreation too early for GetWorld()
	
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &ULevelChangeManager::OnInitialWorldBeginTearDown);
	return true;
}

bool ULevelChangeManager::GetTransitionWorld(TSoftObjectPtr<UWorld>& OutWorld)
{
	const UGameMapsSettings* GameMapsSettings = GetDefault<UGameMapsSettings>();
	if (!GameMapsSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get GameMapsSettings"))
		return false; 
	}

	FSoftObjectPath TransitionMapPath(GameMapsSettings->TransitionMap);
	if (TransitionMapPath.IsValid())
	{
		// Convert the path to a TSoftObjectPtr<ULevel>
		OutWorld = TSoftObjectPtr<UWorld>(TransitionMapPath);
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Transition map is not specified in project settings"))
	return false;
}

void ULevelChangeManager::OnInitialWorldBeginTearDown(UWorld* World)
{
	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);
	FWorldDelegates::OnWorldPreActorTick.AddUObject(this, &ULevelChangeManager::OnTransitionWorldPreActorTick);
}

void ULevelChangeManager::OnTransitionWorldPreActorTick(UWorld* World, ELevelTick LevelTick, float DeltaSeconds)  
{
	FWorldDelegates::OnWorldPreActorTick.RemoveAll(this);
	
	OnTransitionLevelOpened.Broadcast();
	CreateTransitionWidget(TransitionWidgetClass);
	
	LoadTargetWorld(SoftTargetWorld, bAutoTransition, AutoTransitionDelay);
}

bool ULevelChangeManager::CreateTransitionWidget(TSoftClassPtr<UUserWidget> WidgetClass)
{
	if (WidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("TransitionWidgetClass is null"));
		return false;
	}
	
	UClass* WidgetClassLoaded = WidgetClass.LoadSynchronous();
	if (!WidgetClassLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load widget class"));
		return false;
	}
	
	UWorld* WorldContext = GetWorld();
	if (!WorldContext)
	{
		UE_LOG(LogTemp, Error, TEXT("World context is null"));
		return false;
	}
	
	//Spawn new
	Widget = CreateWidget<UUserWidget>(WorldContext, WidgetClassLoaded);
	if (!Widget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to creare a widget of class: %s"), *WidgetClass->GetPathName()); 
		return false;
	}
	Widget->AddToViewport(1);
	
	return true;
}

///// Target World:

void ULevelChangeManager::LoadTargetWorld(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay)
{
    if (InTargetWorld.IsNull())
    {
        UE_LOG(LogTemp, Error, TEXT("Null InTargetWorld"));
        return;
    }

    // Get the StreamableManager from the AssetManager
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// Create a delegate to be called when the asset is loaded
	FStreamableDelegate LoadCompleteDelegate = FStreamableDelegate::CreateUObject(this, &ULevelChangeManager::OnTargetLevelLoaded);
	
    // Start the async load
    CurrentLoadHandle = StreamableManager.RequestAsyncLoad(InTargetWorld.ToSoftObjectPath(), LoadCompleteDelegate);

    if (CurrentLoadHandle.IsValid())
    {
    	// Set up a timer to update the progress
    	GetWorld()->GetTimerManager().SetTimer(ProgressUpdateTimerHandle, this, &ULevelChangeManager::UpdateLoadProgress, 0.1f, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to start async load for target world"));
    }
}

void ULevelChangeManager::UpdateLoadProgress()
{
	if (CurrentLoadHandle.IsValid())
	{
		float Progress = CurrentLoadHandle->GetProgress();
		UE_LOG(LogTemp,Display, TEXT("Progress is %f"), Progress);

		if (CurrentLoadHandle->HasLoadCompleted())
		{
			// Stop the progress update timer
			GetWorld()->GetTimerManager().ClearTimer(ProgressUpdateTimerHandle);
		}
	}
}

void ULevelChangeManager::OnTargetLevelLoaded()
{
	OnTargetWorldLoaded.Broadcast(false);

	TargetWorld =nullptr;
	TargetWorld = Cast<UWorld>(CurrentLoadHandle->GetLoadedAsset());

	if (!TargetWorld)
	{
		UE_LOG(LogTemp,Error,TEXT("Could not cast loaded asset into UWorld"));
		return;
	}

	if(bAutoTransition)
	{
		//Trigger immediately if there is no delay
		if (AutoTransitionDelay<=0)
		{
			TriggerOpenTargetWorld();
		}
		//There is delay therefore set up Timer:
		// Access the timer manager from the world context
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		// Set a very short timer to simulate a one-frame delay
		FTimerHandle TimerHandle;
		auto DelayedWorldLoad = [this]()
		{
			TriggerOpenTargetWorld();
		};
		TimerManager.SetTimer(TimerHandle, DelayedWorldLoad, AutoTransitionDelay, false);
	}
	else
	{
		if(!OnTriggerTransitionReady.IsBound())
		{
			UE_LOG(LogTemp, Error, TEXT("No function is bound to OnTriggerTransitionReady "));
		}
		OnTriggerTransitionReady.Broadcast();
	}
}

bool ULevelChangeManager::TriggerOpenTargetWorld()
{
	if (!TargetWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetLevel is not set"))
		return false; 
	}

	//Get path of level to load
	FString LevelName = TargetWorld->GetOutermost()->GetPathName();
	
	// Now use UGameplayStatics to open the level
	UGameplayStatics::OpenLevel(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull), *LevelName, true, Options);
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &ULevelChangeManager::OnTargetLevelOpened);
	return true;
}

void ULevelChangeManager::OnTargetLevelOpened(UWorld* World)
{
	FWorldDelegates::OnWorldInitializedActors.RemoveAll(this);
	TargetWorld = nullptr;
	SoftTargetWorld = nullptr;
	Options = FString();
	Widget = nullptr;
	bIsTransitionOngoing = false;
	OnTargetWorldOpened.Broadcast();
}

//// Delegates

bool ULevelChangeManager::IsBound()
{
	return OnTriggerTransitionReady.IsBound();
}
