// All rights reserved KAG 2024


#include "Subsystems/LevelChangeManager.h"

#include "GameMapsSettings.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "UObject/Package.h"
#include "UObject/UObjectIterator.h"

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

void ULevelChangeManager::StartWorldTransition(TSoftObjectPtr<UObject> InTargetWorld, bool bAutoTransition, float AutoTransitionDelay)
{
	if (InTargetWorld.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("InTargetWorld is not valid"));
		return;
	}
	
	//Go to transition level
	if (!OpenTransitionWorld()) { return; }
	
	OnTransitionLevelOpened.Broadcast();
	
	InitializeTransitionWidget();
	
	//I load it with a frame delay because I need to give time for created widget execute its construction and therefore bind to the "OnTriggerTransitionReady" delegate.
	LoadWithFrameDelay(InTargetWorld, bAutoTransition, AutoTransitionDelay);
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
	FString Package;
	if (!TSoftObjectPtrToPackage(TransitionWorld, Package))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get package for transition world"))
		return false;
	}
	if (!TSoftObjectPtrToPackage(TransitionWorld, Package))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get package for transition world"))
		return false;
	}

	OnLevelChangeBegin.Broadcast();
	UGameplayStatics::OpenLevel(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull), *Package);
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

///// Target World:

void ULevelChangeManager::LoadTargetWorld(TSoftObjectPtr<UObject> InTargetWorld, bool bAutoTransition, float AutoTransitionDelay)
{
	FString Package;
	if (!TSoftObjectPtrToPackage(InTargetWorld, Package))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert TSoftObjectPtrToPackage"));
	}

	if (bAutoTransition)
	{
		LoadPackageAsync(Package, FLoadPackageAsyncDelegate::CreateUObject(this, &ULevelChangeManager::OnLevelAsyncLoadedAutoTrigger, AutoTransitionDelay));
	}
	else
	{
		LoadPackageAsync(Package, FLoadPackageAsyncDelegate::CreateUObject(this, &ULevelChangeManager::OnLevelAsyncLoadedNoAutoTrigger));
	}
}

void ULevelChangeManager::LoadWithFrameDelay(TSoftObjectPtr<UObject> InTargetWorld, bool bAutoTransition, float AutoTransitionDelay)
{
	// Access the timer manager from the world context
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    
	// Set a very short timer to simulate a one-frame delay
	FTimerHandle TimerHandle;
	
	const float OneFrameDelay = 0.0001f; // Small delay, practically next frame

	auto DelayedWorldLoad = [this, InTargetWorld, bAutoTransition, AutoTransitionDelay]()
	{
		LoadTargetWorld(InTargetWorld, bAutoTransition, AutoTransitionDelay);
	};
	
	TimerManager.SetTimer(TimerHandle, DelayedWorldLoad, OneFrameDelay, false);
}

void ULevelChangeManager::OnLevelAsyncLoadedNoAutoTrigger(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	OnTargetWorldLoaded.Broadcast(false);
	GetWorldFromPackage(PackageName, LoadedPackage, TargetWorld);
	if(!OnTriggerTransitionReady.IsBound())
	{
		UE_LOG(LogTemp, Error, TEXT("No function is bound to OnTriggerTransitionReady "));
	}
	OnTriggerTransitionReady.Broadcast();
}

void ULevelChangeManager::OnLevelAsyncLoadedAutoTrigger(const FName& PackageName, UPackage* LoadedPackage,
	EAsyncLoadingResult::Type Result, float AutoTransitionDelay)
{
	OnTargetWorldLoaded.Broadcast(true);
	GetWorldFromPackage(PackageName, LoadedPackage, TargetWorld);

	//Trigger immediately if there is no delay
	if (AutoTransitionDelay<=0)
	{
		TriggerTransition();
	}

	//There is delay therefore set up Timer:
	// Access the timer manager from the world context
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	// Set a very short timer to simulate a one-frame delay
	FTimerHandle TimerHandle;
	auto DelayedWorldLoad = [this]()
	{
		TriggerTransition();
	};
	TimerManager.SetTimer(TimerHandle, DelayedWorldLoad, AutoTransitionDelay, false);
}

bool ULevelChangeManager::TriggerTransition()
{
	if (!TargetWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetLevel is not set"))
		return false; 
	}

	//Get path of level to load
	FString LevelName = TargetWorld->GetOutermost()->GetPathName();
	
	// Now use UGameplayStatics to open the level
	UGameplayStatics::OpenLevel(GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull), *LevelName);
//	UE_LOG(LogTemp, Warning, TEXT("TargetLevel Should open"))
	//World transition finished therefor unset TargetWorld
	TargetWorld = nullptr;

	/*if(TransitionWidget)
	{
		TransitionWidget->RemoveFromParent();
		//TransitionWidget->RemoveFromRoot();
	}*/
	OnTargetWorldOpened.Broadcast();
	return true;
}

//// Widget:

bool ULevelChangeManager::InitializeTransitionWidget()
{
	if (TransitionWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("TransitionWidgetClass is null"));
		return false;
	}

	TSoftObjectPtr<UWorld> TransitionWorld;
	if (!GetTransitionWorld(TransitionWorld))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find transition level"))
		return false;
	}
	
	UClass* WidgetClassLoaded = TransitionWidgetClass.LoadSynchronous();
	//Spawn new
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(TransitionWorld.LoadSynchronous(), WidgetClassLoaded);
	if (!UserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to creare a widget of class: %s"), *TransitionWidgetClass->GetPathName()); 
		return false;
	}
	UserWidget->AddToViewport(1);
	
	return true;
}

//// Utility:

bool ULevelChangeManager::GetWorldFromPackage(const FName& PackageName, UPackage* LoadedPackage, UWorld*& OutWorld)
{
	// Check if the loaded package is valid
	if (!LoadedPackage)
	{
		UE_LOG(LogTemp, Error, TEXT("Async loaded package is invalid"))
		return false;
	}
    
	// Iterate over all objects in the package
	for (TObjectIterator<UObject> It; It; ++It)
	{
		// Check if the object is in the target package and if it is a level
		if (It->IsIn(LoadedPackage) && It->IsA<UWorld>())
		{
			OutWorld = Cast<UWorld>(*It);
			return true;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("No level found in package"))
	return false; // No level found in the package
}

bool ULevelChangeManager::TSoftObjectPtrToPackage(const TSoftObjectPtr<UObject>& World, FString& OutPackage)
{
	//Asset: "/Game/OtherMaps/Level1.Level1"
	//Package / MountedPath: "/Game/OtherMaps/Level1"
	
	if (World.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot get TargetWorld Package because this World is null"));
		return false;
	}
	
	FString LString;
	FString RString;
	if (!World.ToString().Split(".", &LString, &RString, ESearchCase::IgnoreCase,ESearchDir::FromStart))
	{
		return false;
	}
	OutPackage = LString;
	return true;
}

bool ULevelChangeManager::IsBound()
{
	return OnTriggerTransitionReady.IsBound();
}

