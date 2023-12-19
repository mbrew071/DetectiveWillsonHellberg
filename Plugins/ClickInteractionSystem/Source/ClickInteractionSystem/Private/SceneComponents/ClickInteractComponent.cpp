// All rights reserved, HelloIT Norweskie


#include "SceneComponents/ClickInteractComponent.h"

#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"

#include "Components/WidgetComponent.h"
#include "Objects/InteractActions.h"
#include "Objects/InteractActionsManager.h"

//////////////////////////////////////////////// General /////////////////////////////////////////////////////////////
UClickInteractComponent::UClickInteractComponent()
{
	//Idk This doesnt work, no idea how to default it to a blueprint widget class
	//WidgetClass = TSoftClassPtr<UUserWidget>(FString::Printf(TEXT("%s/Plugins/ClickInteractionSystem/Content/WBP_InteractPin.uasset"), *FPaths::ProjectContentDir()));
	InitClickInteractComponent();
	InitWidgetComponent();
}

void UClickInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	InitComponentTags();
	InitRangeArea();
	RangeAreaAddDynamic();
	SetNoDistanceVisibility();
}

void UClickInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UClickInteractComponent::InitClickInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

//////////////////////////////////////////////// Tags  /////////////////////////////////////////////////////////////

void UClickInteractComponent::InitComponentTags()
{
	if (!ComponentTag.IsValid()) { UE_LOG(LogTemp, Error, TEXT("ComponentTag is invalid tag in ClickInteractComponent")) }
	ComponentTags.Add(ComponentTag.GetTagName());
}

//////////////////////////////////////////////// Collision /////////////////////////////////////////////////////////////

void UClickInteractComponent::InitRangeArea()
{
	if(!RangeAreaTag.IsValid()) { return; }
	
	AActor* Owner = this->GetOwner();
	TArray<UActorComponent*> Components = Owner->GetComponentsByTag(UShapeComponent::StaticClass(), RangeAreaTag.GetTagName());

	RangeArea.Empty();
	for (auto& Element : Components)
	{
		RangeArea.Add(Element);
	}
}

 
void UClickInteractComponent::OnComponentRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCheckDistance) {return;}
	if(!OtherActor->ActorHasTag(OverlapTarget.GetTagName())) {return;}
	ActorsInRange.Add(OtherActor);
	UpdateWidgetVisibility();
}

void UClickInteractComponent::OnComponentRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bCheckDistance) {return;}
	if(!OtherActor->ActorHasTag(OverlapTarget.GetTagName())) {return;}
	ActorsInRange.Remove(OtherActor);
	UpdateWidgetVisibility();
}

void UClickInteractComponent::RangeAreaAddDynamic()
{
	//TODO try do these on construction
	for (auto& Element : RangeArea)
	{
		UShapeComponent* ShapeComponent = static_cast<UShapeComponent*>(Element);
		if(ShapeComponent)
		{
			ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeBeginOverlap);
			ShapeComponent->OnComponentEndOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeEndOverlap);
		}
	}
}

void UClickInteractComponent::UpdateWidgetVisibility()
{
	if(ActorsInRange.IsEmpty())
	{
		SetWidgetVisibility(false);
	}
	else
	{
		SetWidgetVisibility(true);
	}
}

void UClickInteractComponent::TryInteractWith(const AActor* InteractingCharacter)
{
	if(!InteractingCharacter) { return; }
	if (!InteractActions.LoadSynchronous()) { return; }
	if (!InteractionType.IsValid()) { return; }
	
	//Is the character close enough?
	if(bCheckDistance)
	{
		if (!ActorsInRange.Contains(InteractingCharacter)) { return; }
	}

	//TODO FIX InteractActionsManager. Atm it crashes engine. To reproduce:
	//1. Run engine
	//2. Recompile Blueprint child of UInteractActions
	//3. Try to interact with something
	  
	//UInteractActions* Actions = UInteractActionsManager::GetInteractActions(InteractActions);
	//if (!Actions) { return; }
	//UE_LOG(LogTemp, Error, TEXT("NewInstance is not valid"))
	//Actions->PerformInteraction(InteractingCharacter, GetOwner(), InteractionType);

	UInteractActions* NewInstance = NewObject<UInteractActions>(GetTransientPackage(), InteractActions.LoadSynchronous(), NAME_Object);
	NewInstance->PerformInteraction(InteractingCharacter, GetOwner(), InteractionType);
	//OnInteractionCompleted.Broadcast();
}

void UClickInteractComponent::TryInteractWith_I_Implementation(const AActor* InteractingCharacter)
{
	//IClickInteractComponent_I::TryInteractWith_I_Implementation(InteractingCharacter, OnInteractionCompleted);
	TryInteractWith(InteractingCharacter);
}

void UClickInteractComponent::InitWidgetComponent()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(this);
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

bool UClickInteractComponent::SetWidgetVisibility(const bool bNewVisibility)
{
	
	if (!WidgetComponent) { return false; }
	if (!bNewVisibility)
	{
		//remove current widget 
		if (UserWidget) {UserWidget->RemoveFromParent();}
		if (UserWidget) {UserWidget->Destruct();}
		
		WidgetComponent->SetWidget(nullptr);
		return true;
	}

	//Try load widget class
	UClass* WidgetClassLoaded = WidgetClass.LoadSynchronous();
	if (!WidgetClassLoaded) { return false; }
	
	if(bNewVisibility)
	{
		//remove Current
		if (UserWidget) {UserWidget->RemoveFromParent();}
		if (UserWidget) {UserWidget->Destruct();}

		//Spawn new
		UserWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClassLoaded);
		if (!UserWidget) { return false; }

		//Spawned Succeeded therefore we initialize:
		WidgetComponent->SetWidget(UserWidget);
		return true;
	}
	
	//Spawn failed therefore we cleanup
	WidgetComponent->SetWidget(nullptr);
	return false;
}

void UClickInteractComponent::SetNoDistanceVisibility()
{
	if (!bCheckDistance)
	{
		SetWidgetVisibility(true);
	}
}
