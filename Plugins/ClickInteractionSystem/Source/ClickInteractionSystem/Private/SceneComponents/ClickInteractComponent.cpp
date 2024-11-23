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
	DisableTick();
	InitWidgetComponent();
}

void UClickInteractComponent::DisableTick()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UClickInteractComponent::InitWidgetComponent()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(this);
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(32.0,32.0));
}

//////////////////////////////////////////////// BeginPlay  ////////////////////////////////////////////////////////////
void UClickInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	InitComponentTag();
	InitRangeArea();
	InitWidgetClass();
	RangeAreaAddDynamic();
	SetNoDistanceVisibility();
}

bool UClickInteractComponent::InitComponentTag()
{
	if (!GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->InteractComponent.IsValid())
	{
		return false;
	}
	ComponentTags.Add(GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->InteractComponent.GetTagName());
	return true;
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

void UClickInteractComponent::InitWidgetClass()
{
	if ( WidgetClass.IsNull())
	{
		WidgetClass = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->DefaultWidget;
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
	ActorsInRange.Add(OtherActor);

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
	if (!WidgetComponent) { return; }
	
	if (WidgetClass.IsNull())
	{
		UE_LOG(LogTemp,Warning, TEXT("Cannot load widget because widget classes are not specified."))
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
	if (!WidgetComponent) { return; }
	WidgetComponent->SetWidgetClass(nullptr);
}

//////////////////////////////////////////////// Interaction /////////////////////////////////////////////

void UClickInteractComponent::TryInteractWith_I_Implementation(const AActor* InteractingCharacter)
{
	TryInteractWith(InteractingCharacter);
}

void UClickInteractComponent::TryInteractWith(const AActor* InteractingCharacter)
{
	if (bInteractionOngoing) { return;}
	if(!InteractingCharacter) { return; }
	
	TSoftClassPtr<UInteractActions> InteractActions = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UClickInteractController>()->InteractActions;;
	
	if (!InteractActions.LoadSynchronous()) { return; }
	if (!InteractionType.IsValid()) { return; }
	
	if(ShouldCheckRange())
	{
		if (!ActorsInRange.Contains(InteractingCharacter))
		{
			//Actor that tries to interact is not in range
			return;
		}
	}
	bInteractionOngoing = true;
	
	UInteractActions* NewInstance = NewObject<UInteractActions>(this->GetOwner()->GetWorld(), InteractActions.LoadSynchronous());

	NewInstance->OnInteractionEnd.AddDynamic(this, &UClickInteractComponent::OnInteractionActionEnd);
	NewInstance->OnInteractionBegin.AddDynamic(this, &UClickInteractComponent::OnInteractionActionBegin);
	//NewInstance->OnInteractionProgress.AddDynamic(this, &UClickInteractComponent::OnInteractionActionProgress);
		
	NewInstance->PerformInteraction(InteractingCharacter, GetOwner(), InteractionType);
}

void UClickInteractComponent::OnInteractionActionEnd(const bool bSuccess)
{
//	OnInteractionProgress.Clear();
	OnInteractionBegin.Clear();
	OnInteractionEnd.Broadcast(bSuccess);
	OnInteractionEnd.Clear();
	bInteractionOngoing = false;
}

void UClickInteractComponent::OnInteractionActionBegin()
{
	OnInteractionBegin.Broadcast();
	OnInteractionBegin.Clear();
}

/*void UClickInteractComponent::OnInteractionActionProgress(const int32 Progress)
{
	OnInteractionProgress.Broadcast(Progress);
}*/
