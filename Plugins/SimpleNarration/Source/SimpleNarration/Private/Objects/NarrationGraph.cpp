// © 2024 KAG. All rights reserved.


#include "Objects/NarrationGraph.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/NarrationWidget.h"
#include "Objects/GraphNarrationCustomAction.h"
#include "ActorComponents/GraphNarrationTarget.h"
#include "Subsystems/GraphNarrationController.h"
//#include "Blueprint/UserWidget.h"
#include "Subsystems/NarrationVariablesContainer.h"
#include "Engine/GameInstance.h"
#include "Engine/Level.h"
#include "Engine/World.h"

UWorld* UNarrationGraph::GetWorld() const
{
	// CDO objects do not belong to a world
	// If the actors outer is destroyed or unreachable we are shutting down and the world should be nullptr
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("Actor: %s has a null OuterPrivate in AActor::GetWorld()"), *GetFullName()) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (ULevel* Level = GetTypedOuter<ULevel>())
		{
			return Level->OwningWorld;
		}
	}
	
	UObject* Outer = GetOuter();
	if (UWorld* World = Cast<UWorld>(Outer))
	{
		return World;
	}
	/*if (AActor* Actor = Cast<AActor>(Outer))
	{
		return Actor->GetWorld();
	}*/
	/*if (UActorComponent* ActorComponent = Cast<UActorComponent>(Outer))
	{
		return ActorComponent->GetOwner()->GetWorld();
	}
	if (ULevel* Level = Cast<ULevel>(Outer))
	{
		return Level->OwningWorld;
	}*/
	return nullptr;
	
	/*
	//If this class has a child which is a blueprint class.
	//AND when in editor I press RMB on graph to search for functions
	//This overriden "GetWorld()" will be called ( as engine will be searching for functions )
	//In this situation if this function would return a nullptr the engine would crash.
	if (GEngine)
	{
		FWorldContext* WorldContext1 = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		return WorldContext1->World();
	}
	
	return nullptr;*/
}

//////////////////// API for other classes ////////////////////////////////////////////////

void UNarrationGraph::FailAllActions(const FGameplayTag Reason)
{
	PlayScreenNarrationFailed_Implementation(Reason);
	PlaySceneNarrationFailed_Implementation(Reason);
	DisplayOptionsFailed_Implementation(Reason);
	ActionCustomFailed_Implementation(Reason);
}

/////////////////////////////// ACTIONS /////////////////////////////////////////////////////

void UNarrationGraph::ActionDisplayOptions(ENarrationPinType& Outcome,const TArray<FNarrationOption> Options, int32& ChosenOption, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo,  const float DecisionTime)
{
	FailAllActions(FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.NewActionBegan"));
	
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (FPlayNarrationLatentAction* ExistingAction = LatentActionManager.FindExistingAction<FPlayNarrationLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
		{
			// If there's an existing action with the same UUID, cancel it
			ExistingAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.AnotherActionStarted"));
		}

		// Create and register the new latent action
		CurrentDisplayOptionsAction = new FDisplayOptionsLatentAction(Outcome, FailureReason, ChosenOption, LatentInfo);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, CurrentDisplayOptionsAction);

		if(bActive == false)
		{
			CurrentDisplayOptionsAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.GraphCancelled"));
			return;
		}

		if (ScreenWidget == nullptr)
		{
			CreateScreenNarrationWidget(ScreenWidget);
		}
		
		if (ScreenWidget)
		{
			if (ScreenWidget->GetClass()->ImplementsInterface(UNarrationWidget::StaticClass()))
			{
				INarrationWidget::Execute_DisplayOptions(ScreenWidget, this, Options, DecisionTime);
			}
			else
			{
				UE_LOG(LogTemp,Fatal, TEXT("widget doesn't implement SimpleNarrationWidget interface. we cannot continue"));	
			}
		}
		else
		{
			UE_LOG(LogTemp,Fatal, TEXT("widget is invalid. we cannot continue"));
		}
	}
}

void UNarrationGraph::ActionPlayScreenNarration(ENarrationPinType& Outcome, const TArray<FScreenNarrationPiece> Phrases, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo)
{
	FailAllActions(FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.NewActionBegan"));
	if( bActive == false ) {return;}
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (FPlayNarrationLatentAction* ExistingAction = LatentActionManager.FindExistingAction<FPlayNarrationLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
		{
			// If there's an existing action with the same UUID, cancel it
			ExistingAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.AnotherActionStarted"));
		}

		// Create and register the new latent action
		CurrentPlayNarrationAction = new FPlayNarrationLatentAction(Outcome, FailureReason, LatentInfo);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, CurrentPlayNarrationAction);
        
		if(bActive == false)
		{
			CurrentPlayNarrationAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.GraphCancelled"));
			return;
		}

		if (ScreenWidget == nullptr)
		{
			CreateScreenNarrationWidget(ScreenWidget);
		}
		
		if (ScreenWidget)
		{
			if ( ScreenWidget->GetClass()->ImplementsInterface(UNarrationWidget::StaticClass()))
			{
				INarrationWidget::Execute_PlayScreenNarration(ScreenWidget,this,Phrases);
			}
			else
			{
				UE_LOG(LogTemp,Fatal, TEXT("widget doesn't implement SimpleNarrationWidget interface. we cannot continue"));	
			}
		}
		else
		{
			UE_LOG(LogTemp,Fatal, TEXT("widget is invalid. we cannot continue"));
		}
	}
}

void UNarrationGraph::ActionPlaySceneNarration(ENarrationPinType& Outcome, AActor*& Actor, const TArray<FSceneNarrationPiece> Phrases, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo)	
{
	FailAllActions(FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.NewActionBegan"));
	if( bActive == false ) {return;}
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (FPlaySceneNarrationLatentAction* ExistingAction = LatentActionManager.FindExistingAction<FPlaySceneNarrationLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
		{
			// If there's an existing action with the same UUID, cancel it
			ExistingAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.AnotherActionStarted"));
		}

		// Create and register the new latent action
		CurrentSceneNarrationAction = new FPlaySceneNarrationLatentAction(Outcome, FailureReason, LatentInfo);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, CurrentSceneNarrationAction);

		if(bActive==false)
		{
			CurrentSceneNarrationAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.GraphCancelled"));
			return;
		}
		
		if (Actor == nullptr)
		{
			PlaySceneNarrationFailed_Implementation(FGameplayTag());
			UE_LOG(LogTemp,Error, TEXT("Cannot play ActionPlaySceneNarration because Actor is not specified"));
			return;
		}

		UGraphNarrationTarget* SceneGraphNarrationSource = Actor->FindComponentByClass<UGraphNarrationTarget>();
		if (SceneGraphNarrationSource == nullptr)
		{
			UE_LOG(LogTemp,Warning, TEXT("This NPC %s doesnt have SceneGraphNarrationSource component so it wont be able to display scene narration"), *Actor->GetFName().ToString());	
			PlaySceneNarrationFailed_Implementation(FGameplayTag());
		}

		SceneWidget = nullptr; //TODO fix this, dont keep widget ref here... but rather in the components. So remove "SceneWidget" from graph 
	
		if (SceneWidget == nullptr)
		{
			CreateSceneNarrationWidget(SceneWidget);

			if (SceneWidget == nullptr)
			{
				UE_LOG(LogTemp,Fatal, TEXT("Failed to spawn scene widget we cannot continue"));
				PlaySceneNarrationFailed_Implementation(FGameplayTag());
			}
			
			SceneGraphNarrationSource->WidgetComponent->SetWidget(SceneWidget);
			SceneGraphNarrationSource->WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			SceneGraphNarrationSource->WidgetComponent->SetDrawAtDesiredSize(true);
		}
		
		if (SceneWidget->GetClass()->ImplementsInterface(UNarrationWidget::StaticClass()))
		{
			SceneWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			INarrationWidget::Execute_PlaySceneNarration(SceneWidget,this,Phrases);
		}
		else
		{
			UE_LOG(LogTemp,Fatal, TEXT("widget doesn't implement SimpleNarrationWidget interface. we cannot continue"));
			PlaySceneNarrationFailed_Implementation(FGameplayTag());
		}
	}
}

void UNarrationGraph::ActionCustom(ENarrationPinType& Outcome, const FGameplayTag ActionTag, const UObject* DTO, FGameplayTag& FailureReason, FLatentActionInfo LatentInfo)
{
	FailAllActions(FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.NewActionBegan"));
	if( bActive == false ) {return;}
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (FPlayNarrationLatentAction* ExistingAction = LatentActionManager.FindExistingAction<FPlayNarrationLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
		{
			// If there's an existing action with the same UUID, cancel it
			ExistingAction->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.AnotherActionStarted"));
		}

		// Create and register the new latent action
		CurrentActionCustom = new FActionCustomLatentAction(Outcome, FailureReason, DTO, LatentInfo);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, CurrentActionCustom);

		if(bActive==false)
		{
			CurrentActionCustom->FinishAction(ENarrationPinType::Failed, FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.GraphCancelled"));
			return;
		}
		
		UGraphNarrationController* Controller = GetWorld()->GetGameInstance()->GetSubsystem<UGraphNarrationController>();
		UGraphNarrationCustomAction* NewInstance = NewObject<UGraphNarrationCustomAction>(this, Controller->ActionCustom.LoadSynchronous(), NAME_Object);
		NewInstance->PerformCustomAction(this, ScreenWidget, ActionTag, DTO);	
	}
}

void UNarrationGraph::ActionEndGraph(const FGameplayTag Reason)
{
	if (bActive == false)
	{
		return;
	}
	bActive= false;
	FailAllActions(FGameplayTag::RequestGameplayTag("GraphNarration.FailureReason.GraphEnded"));
	
	if (ScreenWidget)
	{
		if ( ScreenWidget->GetClass()->ImplementsInterface(UNarrationWidget::StaticClass()))
		{
			INarrationWidget::Execute_EndNarration(ScreenWidget);
			ScreenWidget=nullptr;
		}
		else
		{
			UE_LOG(LogTemp,Fatal, TEXT("ScreenWidget doesn't implement SimpleNarrationWidget interface. we cannot continue"));	
		}
	}
	if(SceneWidget)
	{
		if (SceneWidget->GetClass()->ImplementsInterface(UNarrationWidget::StaticClass()))
		{
			INarrationWidget::Execute_EndNarration(SceneWidget);
			SceneWidget=nullptr;
		}
		else
		{
			UE_LOG(LogTemp,Fatal, TEXT("SceneWidget doesn't implement SimpleNarrationWidget interface. we cannot continue"));	
		}
	}
	OnGraphNarrationEnd.ExecuteIfBound();
	OnGraphNarrationEnd.Clear();
}

////////////////////////////// Interface functions /////////////////////////////////////////////

void UNarrationGraph::PlayScreenNarrationCompleted_Implementation()
{
	if (CurrentPlayNarrationAction)
	{
		CurrentPlayNarrationAction->FinishAction(ENarrationPinType::Completed);
		CurrentPlayNarrationAction = nullptr;
	}
}

void UNarrationGraph::PlayScreenNarrationFailed_Implementation(const FGameplayTag& FailureReason)
{
	if (CurrentPlayNarrationAction)
	{
		CurrentPlayNarrationAction->FinishAction(ENarrationPinType::Failed, FailureReason);
		CurrentPlayNarrationAction = nullptr;
	}
}

void UNarrationGraph::PlaySceneNarrationCompleted_Implementation()
{
	if (SceneWidget)
	{
		SceneWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CurrentSceneNarrationAction)
	{	
		CurrentSceneNarrationAction->FinishAction(ENarrationPinType::Completed);
		CurrentSceneNarrationAction = nullptr;
	}
}

void UNarrationGraph::PlaySceneNarrationFailed_Implementation(const FGameplayTag& FailureReason)
{
	if (SceneWidget)
	{
		SceneWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CurrentSceneNarrationAction)
	{
		CurrentSceneNarrationAction->FinishAction(ENarrationPinType::Failed, FailureReason);
		CurrentSceneNarrationAction = nullptr;
	}
}

void UNarrationGraph::DisplayOptionsCompleted_Implementation(const int32& ChosenOptions)
{
	if (CurrentDisplayOptionsAction)
	{
		CurrentDisplayOptionsAction->FinishAction(ENarrationPinType::Completed, FGameplayTag(), ChosenOptions);
		CurrentDisplayOptionsAction = nullptr;
	}
}

void UNarrationGraph::DisplayOptionsFailed_Implementation(const FGameplayTag& FailureReason)
{
	if (CurrentDisplayOptionsAction)
	{
		CurrentDisplayOptionsAction->FinishAction(ENarrationPinType::Failed, FailureReason);
		CurrentDisplayOptionsAction = nullptr;
	}
}

void UNarrationGraph::ActionCustomCompleted_Implementation()
{
	if (CurrentActionCustom)
	{
		CurrentActionCustom->FinishAction(ENarrationPinType::Completed);
		CurrentActionCustom = nullptr;
	}
}

void UNarrationGraph::ActionCustomFailed_Implementation(const FGameplayTag& FailureReason)
{
	if (CurrentActionCustom)
	{
		CurrentActionCustom->FinishAction(ENarrationPinType::Failed, FailureReason);
		CurrentActionCustom = nullptr;
	}
}

/////////////////////////////// Actor narration variables ////////////////////////////////////////

void UNarrationGraph::SetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, const bool Value)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" SetNarrationBool wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->SetNarrationBool(Actor, VariableName, Value );
}

void UNarrationGraph::GetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, bool& Value, const bool bCreateIfNotFound)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" GetNarrationBool wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->GetNarrationBool(Actor, VariableName, bSuccess, Value,  bCreateIfNotFound);
}

void UNarrationGraph::GetNarrationBool_Pure(UPARAM(ref) AActor* Actor, const FName VariableName, const FGameplayTag Scope,  bool& bSuccess, bool& Value, const bool bCreateIfNotFound)
{
	GetNarrationBool(Actor, VariableName, bSuccess, Value, bCreateIfNotFound);
}

void UNarrationGraph::RemoveNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" RemoveNarrationBool wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->RemoveNarrationBool(Actor, VariableName);
}

void UNarrationGraph::SetNarrationInt(UPARAM(ref) AActor* Actor, const FName Name,  const int Value)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" SetNarrationInt wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->SetNarrationInt(Actor, Name, Value);
}

void UNarrationGraph::GetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" GetNarrationInt wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->GetNarrationInt(Actor, VariableName, bSuccess, Value,  bCreateIfNotFound);
}

void UNarrationGraph::GetNarrationInt_Pure(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	GetNarrationInt(Actor, VariableName, bSuccess,Value,bCreateIfNotFound);
}

void UNarrationGraph::RemoveNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT(" RemoveNarrationInt wil lfail because provided Actor is invalid"))
		return;
	}
	
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();
	
	Variables->RemoveNarrationInt(Actor, VariableName );
}

/////////////////////////////// Global narration variables ////////////////////////////////////////
void UNarrationGraph::SetNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope,  const bool Value)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->SetNarrationBoolGlobal(VariableName, Value, Scope);
}

void UNarrationGraph::GetNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, bool& Value, const bool bCreateIfNotFound)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->GetNarrationBoolGlobal(VariableName, bSuccess, Value, Scope, bCreateIfNotFound);
}

void UNarrationGraph::GetNarrationBoolGlobal_Pure(const FName VariableName, const FGameplayTag Scope,  bool& bSuccess, bool& Value, const bool bCreateIfNotFound)
{
	GetNarrationBoolGlobal(VariableName, Scope, bSuccess, Value, bCreateIfNotFound);
}

void UNarrationGraph::RemoveNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->RemoveNarrationBoolGlobal(VariableName, Scope);
}

void UNarrationGraph::SetNarrationIntGlobal(const FName Name, const FGameplayTag Scope, const int Value)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->SetNarrationIntGlobal(Name, Value, Scope);
}

void UNarrationGraph::GetNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();

	Variables->GetNarrationIntGlobal(VariableName, Scope, bSuccess, Value, bCreateIfNotFound);
}

void UNarrationGraph::GetNarrationIntGlobal_Pure(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	GetNarrationIntGlobal(VariableName, Scope,bSuccess,Value,bCreateIfNotFound);
}

void UNarrationGraph::RemoveNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope)
{
	UNarrationVariablesContainer* Variables = GetWorld()->GetGameInstance()->GetSubsystem<UNarrationVariablesContainer>();
	
	Variables->RemoveNarrationIntGlobal(VariableName, Scope);
}

///////////////////////////// Get Narration actor

void UNarrationGraph::GetNarrationActorOfClass(bool& bSuccess, const TSoftClassPtr<AActor> Class, AActor*& Actor, const int32 SearchedNumber)
{
	if (NarrationActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NarrationActors array is empty, cannot return an actor"))
		bSuccess = false;
		return;
	}
	
	int32 FoundCount = -1;
	for(auto& Element : NarrationActors)
	{
		if (TSoftClassPtr<AActor>(Element->GetClass()) == Class)
		{
			FoundCount += 1;
			if (FoundCount == SearchedNumber)
			{
				Actor = Element;
				bSuccess = true;
				return;
			}
		}
	}
	bSuccess = false;
	return;
}

void UNarrationGraph::GetNarrationActorOfClass_Pure(bool& bSuccess, const TSoftClassPtr<AActor> Class, AActor*& Actor, const int32 SearchedNumber)
{
	GetNarrationActorOfClass(bSuccess, Class, Actor, SearchedNumber);
}

void UNarrationGraph::GetNarrationActor(bool& bSuccess, const int32 Index, AActor*& Actor)
{
	if (NarrationActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NarrationActors array is empty, cannot return an actor"))
		bSuccess = false;
		return;
	}
	if (NarrationActors.Num()-1 < Index)
	{
		UE_LOG(LogTemp, Error, TEXT("Requested index doesn't exist. The last actor will be returned"));
		Actor = NarrationActors.Last();
		bSuccess = false;
		return;
	}
	Actor = NarrationActors[Index];
	bSuccess = true;
	return;
}

void UNarrationGraph::GetNarrationActor_Pure(bool& bSuccess, const int32 Index, AActor*& Actor)
{
	return GetNarrationActor(bSuccess, Index, Actor);
}

void UNarrationGraph::GetNarrationActorWithTag(bool& bSuccess, const FName Tag, AActor*& Actor, const int32 SearchedNumber)
{
	if (NarrationActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NarrationActors array is empty, cannot return an actor"))
		bSuccess = false;
		return;
	}
	
	int32 FoundCount = -1;
	for(auto& Element : NarrationActors)
	{
		if (Element->ActorHasTag(Tag))
		{
			FoundCount += 1;
			if (FoundCount == SearchedNumber)
			{
				Actor = Element;
				bSuccess = true;
				return;
			}
		}
	}
	bSuccess = false;
	return;
}

void UNarrationGraph::GetNarrationActorWithTag_Pure(bool& bSuccess, const FName Tag,AActor*& Actor, const int32 SearchedNumber)
{
	GetNarrationActorWithTag(bSuccess, Tag, Actor, SearchedNumber);
}

void UNarrationGraph::GetNarrationActorWithInterface(bool& bSuccess, TSubclassOf<UInterface> Interface, AActor*& Actor, const int32 SearchedNumber)
{
	if (NarrationActors.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("NarrationActors array is empty, cannot return an actor"))
		bSuccess = false;
		return;
	}

	if (!Interface)
	{
		bSuccess = false;
		return;
	}
	
	int32 FoundCount = -1;
	for(auto& Element : NarrationActors)
	{
		if (Element->GetClass()->ImplementsInterface(Interface))
		{
			FoundCount += 1;
			if (FoundCount == SearchedNumber)
			{
				Actor = Element;
				bSuccess = true;
				return;
			}
		}
	}
	bSuccess = false;
	return;
}

void UNarrationGraph::GetNarrationActorWithInterface_Pure(bool& bSuccess, TSubclassOf<UInterface> Interface, AActor*& Actor, const int32 SearchedNumber)
{
	GetNarrationActorWithInterface(bSuccess, Interface, Actor, SearchedNumber);
}

/////////// Create widgets 

bool UNarrationGraph::CreateScreenNarrationWidget(UUserWidget*& OutWidget)
{
	UGraphNarrationController* Controller = GetWorld()->GetGameInstance()->GetSubsystem<UGraphNarrationController>();

	if (Controller==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get USimpleNarrationController subsystem"));
		return false;
	}
	
	if(Controller->ScreenWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Widget class is invalid"));
		return false;
	}
	
	UClass* WidgetClassLoaded = Controller->ScreenWidgetClass.LoadSynchronous();
	if (WidgetClassLoaded == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load widget class"));
		return false;
	}
	
	UWorld* WorldContext = GetWorld();
	if (WorldContext == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("World context is null"));
		return false;
	}
	
	//Spawn new
	UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(WorldContext, WidgetClassLoaded);
	if (!CreatedWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to creare a widget of class: %s"), *WidgetClassLoaded->GetPathName()); 
		return false;
	}
	//TODO change AddToViewport 
	CreatedWidget->AddToViewport(0);
	OutWidget = CreatedWidget;
	return true;
}

bool UNarrationGraph::CreateSceneNarrationWidget(UUserWidget*& OutWidget)
{
	UGraphNarrationController* Controller = GetWorld()->GetGameInstance()->GetSubsystem<UGraphNarrationController>();

	if (Controller==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get USimpleNarrationController subsystem"));
		return false;
	}
	
	if(Controller->SceneWidgetClass.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("Widget class is invalid"));
		return false;
	}
	
	UClass* WidgetClassLoaded = Controller->SceneWidgetClass.LoadSynchronous();
	if (WidgetClassLoaded == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load widget class"));
		return false;
	}
	
	UWorld* WorldContext = GetWorld();
	if (WorldContext == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("World context is null"));
		return false;
	}
	
	//Spawn new
	UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(WorldContext, WidgetClassLoaded);
	if (!CreatedWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to creare a widget of class: %s"), *WidgetClassLoaded->GetPathName()); 
		return false;
	}
	OutWidget = CreatedWidget;
	return true;
}
