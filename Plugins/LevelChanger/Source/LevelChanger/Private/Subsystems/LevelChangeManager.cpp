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

void ULevelChangeManager::StartWorldTransition(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay)
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
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &ULevelChangeManager::OnTransitionWorldCreation);
	return true;
}
void ULevelChangeManager::OnTransitionWorldCreation(UWorld* World)
{
	//Unbind
	FWorldDelegates::OnPostWorldCreation.RemoveAll(this);
	if (World)
	{
		if (World)
		{
			// Bind to the OnWorldBeginPlay delegate
			World->OnWorldBeginPlay.AddUObject(this, &ULevelChangeManager::OnTransitionWorldBeginPlay);
		}
	}
}

void ULevelChangeManager::OnTransitionWorldBeginPlay()
{
	OnTransitionLevelOpened.Broadcast();
	CreateTransitionWidget(TransitionWidgetClass);

	//I load it with a frame delay because I need to give time for created widget execute its construction and therefore bind to the "OnTriggerTransitionReady" delegate.
	//TODO Check if the OnStart override for widget will be early enough to do these binds. And if yes then I wont have to do the frame delay before load
	LoadWithFrameDelay(SoftTargetWorld, bAutoTransition, AutoTransitionDelay);
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

void ULevelChangeManager::LoadTargetWorld(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay)
{
	FString Package;
	if (!TSoftObjectPtrToPackage(InTargetWorld, Package))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert TSoftObjectPtrToPackage"));
	}

	if (bInAutoTransition)
	{
		LoadPackageAsync(Package, FLoadPackageAsyncDelegate::CreateUObject(this, &ULevelChangeManager::OnLevelAsyncLoadedAutoTrigger, InAutoTransitionDelay));
	}
	else
	{
		LoadPackageAsync(Package, FLoadPackageAsyncDelegate::CreateUObject(this, &ULevelChangeManager::OnLevelAsyncLoadedNoAutoTrigger));
	}
}

void ULevelChangeManager::LoadWithFrameDelay(TSoftObjectPtr<UObject> InTargetWorld, bool bInAutoTransition, float InAutoTransitionDelay)
{
	// Access the timer manager from the world context
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    
	// Set a very short timer to simulate a one-frame delay
	FTimerHandle TimerHandle;
	
	const float OneFrameDelay = 0.0001f; // Small delay, practically next frame

	auto DelayedWorldLoad = [this, InTargetWorld, bInAutoTransition, InAutoTransitionDelay]()
	{
		LoadTargetWorld(InTargetWorld, bInAutoTransition, InAutoTransitionDelay);
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
	EAsyncLoadingResult::Type Result, float InAutoTransitionDelay)
{
	OnTargetWorldLoaded.Broadcast(true);
	GetWorldFromPackage(PackageName, LoadedPackage, TargetWorld);

	//Trigger immediately if there is no delay
	if (InAutoTransitionDelay<=0)
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
	TimerManager.SetTimer(TimerHandle, DelayedWorldLoad, InAutoTransitionDelay, false);
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
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &ULevelChangeManager::OnTargetLevelOpened);
	return true;
}
void ULevelChangeManager::OnTargetLevelOpened(UWorld* World)
{
	FWorldDelegates::OnWorldInitializedActors.RemoveAll(this);
	TargetWorld = nullptr;
	SoftTargetWorld = nullptr;
	bIsTransitionOngoing = false;
	OnTargetWorldOpened.Broadcast();
}

//// Widget:

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
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(WorldContext, WidgetClassLoaded);
	if (!UserWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to creare a widget of class: %s"), *WidgetClass->GetPathName()); 
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


