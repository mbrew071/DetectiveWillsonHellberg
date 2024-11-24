// All rights reserved, KAG 2024

#include "SceneComponents/ClickInteractComponent.h"

#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Objects/InteractActions.h"
#include "Engine/AssetManager.h"
#include "Subsystems/ClickInteractController.h"

//////////////////////////////////////////////// Construction  /////////////////////////////////////////////////////////
UClickInteractComponent::UClickInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(32.0,32.0));
}

//////////////////////////////////////////////// BeginPlay  ////////////////////////////////////////////////////////////
void UClickInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	if(WidgetComponent && !WidgetClass.IsNull())
	{	
		WidgetComponent->AttachToComponent(this,FAttachmentTransformRules::KeepRelativeTransform);
	}
	InitRangeArea();
	RangeAreaAddDynamic();
	SetNoDistanceVisibility();
}

void UClickInteractComponent::InitRangeArea()
{
	AActor* Owner = this->GetOwner();
	TArray<UActorComponent*> Components = Owner->GetComponentsByTag(UShapeComponent::StaticClass(), GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->RangeArea.GetTagName());

	if(Components.Num() == 0)
	{
		//There are no area components therefore this ClickInteract Component can be clicked from any range
		return;
	}
	else
	{
		RangeAreaComponents.Empty();
		for (auto& Element : Components)
		{
			RangeAreaComponents.Add(Element);
		}
	}
}

void UClickInteractComponent::RangeAreaAddDynamic()
{
	//TODO try do these on construction
	for (auto& Element : RangeAreaComponents)
	{
		UShapeComponent* ShapeComponent = static_cast<UShapeComponent*>(Element);
		if(ShapeComponent)
		{
			ShapeComponent->OnComponentBeginOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeAreaBeginOverlap);

			ShapeComponent->OnComponentEndOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeEndOverlap);
		}
	}
}

void UClickInteractComponent::SetNoDistanceVisibility()
{
	if (ShouldCheckRange()== 0)
	{
		ShowWidget();
	}
}

//////////////////////////////////////////////// RangeArea Overlaps /////////////////////////////////////////////

bool UClickInteractComponent::ShouldCheckRange()
{
	return RangeAreaComponents.Num()>0;
}

void UClickInteractComponent::OnComponentRangeAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor->ActorHasTag(GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->OverlapTarget.GetTagName())) {return;}
	ActorsInRange.AddUnique(OtherActor);

	ShowWidget();
}

void UClickInteractComponent::OnComponentRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor->ActorHasTag(GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->OverlapTarget.GetTagName())) {return;}
	ActorsInRange.Remove(OtherActor);

	if(ActorsInRange.IsEmpty())
	{
		HideWidget();
	}
}

//////////////////////////////////////////////// Widget Visibility ////////////////////////////////////////////

void UClickInteractComponent::ShowWidget()
{
	if (WidgetComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot show widget because WidgetComponent is nullptr."))
		return;
	}
	if (WidgetClass.IsNull())
	{
	
		return;
	}
	//TODO ADD NOTE ASYNC LOADING
	// Get the streamable manager
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	// Start async load
	WidgetLoadHandle = StreamableManager.RequestAsyncLoad(WidgetClass.ToSoftObjectPath(),FStreamableDelegate::CreateUObject(this, &UClickInteractComponent::OnWidgetClassLoaded));
}

void UClickInteractComponent::OnWidgetClassLoaded()
{
	if(ActorsInRange.IsEmpty())
	{
		return;;
	}
	
	if (WidgetLoadHandle.IsValid() == false)
	{
		return;
	}

	// Get the loaded class
	UClass* LoadedClass = Cast<UClass>(WidgetLoadHandle->GetLoadedAsset());
    
	if (LoadedClass && WidgetComponent)
	{
		// Set the widget class
		WidgetComponent->SetWidgetClass(LoadedClass);
	}
    
	// Clear the handle
	WidgetLoadHandle.Reset();
}

void UClickInteractComponent::HideWidget()
{
	if (WidgetComponent == nullptr)
	{
		return;
	}
	WidgetComponent->SetWidgetClass(nullptr);
}

//////////////////////////////////////////////// Interaction /////////////////////////////////////////////

void UClickInteractComponent::TryInteractWith_I_Implementation(AActor* InteractingActor)
{
	TryInteractWith(InteractingActor);
}

void UClickInteractComponent::TryInteractWith(AActor* InteractingActor)
{
	if (bInteractionOngoing) { return;}
	if(!InteractingActor) { return; }
	
	if(ShouldCheckRange())
	{
		if (!ActorsInRange.Contains(InteractingActor))
		{
			//Actor that tries to interact is not in range
			return;
		}
	}
	
	bInteractionOngoing = true;
	OnInteractionBegin.Broadcast(InteractingActor);
	OnInteractionEnd.AddDynamic(this, &UClickInteractComponent::OnInteractionActionEnd);
}

void UClickInteractComponent::OnInteractionActionEnd(const bool bSuccess)
{
	bInteractionOngoing = false;
}