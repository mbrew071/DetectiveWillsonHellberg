// © 2024 KAG. All rights reserved.


#include "ActorComponents/GraphNarrationTarget.h"

#include "Blueprint/UserWidget.h"
#include "Objects/NarrationGraph.h"
#include "Subsystems/GraphNarrationController.h"
#include "Components/WidgetComponent.h"
#include "Engine/GameInstance.h"


// Sets default values for this component's properties
UGraphNarrationTarget::UGraphNarrationTarget()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts
void UGraphNarrationTarget::BeginPlay()
{
	Super::BeginPlay();
	//This makes sure that the WidgetComponent is attached to this SceneComponent 
	if (WidgetComponent)
	{
		// Use AttachToComponent instead of SetupAttachment after registration
		WidgetComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

bool UGraphNarrationTarget::BeginNarrationGraph(TArray<AActor*> NarrationActors, TSoftClassPtr<UNarrationGraph> GraphClass, FOnGraphNarrationEnd OnGraphEnd)
{
	if(NarrationActors.IsEmpty())
	{
		UE_LOG(LogTemp,Error,TEXT("Cannot start conversation with no NarrationActors. At least one actor is required"));
		return false;
	}

	EndNarrationGraph(FGameplayTag());
	
	if (GraphClass.IsNull() == false)
	{
		CreateNarrationGraphObject(CurrentGraph, GraphClass);
	}
	else if (DefaultNarrationGraph.IsNull() == false)
	{
		CreateNarrationGraphObject(CurrentGraph, DefaultNarrationGraph);
	}

	if (CurrentGraph==nullptr)
	{
		return false;
	}
	
	//Init variables necessary for Dialogue graph:
	CurrentGraph->NarrationActors = NarrationActors;
	
	CurrentGraph->OnGraphNarrationEnd = OnGraphEnd;

	//Start Narration for this graph
	CurrentGraph->BeginGraph();
	return true;
}

void UGraphNarrationTarget::EndNarrationGraph(const FGameplayTag Reason)
{
	if (CurrentGraph)
	{
		//Cancel currently ongoing 
		CurrentGraph->ActionEndGraph(Reason);
		CurrentGraph = nullptr;
	}
}

bool UGraphNarrationTarget::CreateNarrationGraphObject(UNarrationGraph*& OutGraph, TSoftClassPtr<UNarrationGraph>& Class)
{
	const UGraphNarrationController* Controller = GetWorld()->GetGameInstance()->GetSubsystem<UGraphNarrationController>();

	if(Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get SimpleNarrationController subsystem"));
		return false;
	}
	if(Class.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Narration graph class is null for this component"));
		return false;
	}
	
	UNarrationGraph* CreatedObject = NewObject<UNarrationGraph>(this->GetOwner()->GetWorld(), Class.LoadSynchronous());
	
	if (CreatedObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn new narration graph"));
		return false;
	}
	OutGraph = CreatedObject;
	return true;
}
