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
	InitComponentRange();
	InitClickArea();
	InitWidgetComponent();
}

void UClickInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTags();

	//TODO try do these on construction
	if (ComponentRange)
	{
		ComponentRange->OnComponentBeginOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeBeginOverlap);
		ComponentRange->OnComponentEndOverlap.AddDynamic(this, &UClickInteractComponent::OnComponentRangeEndOverlap);
	}
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

void UClickInteractComponent::InitTags()
{
	if (!ComponentTag.IsValid()) { UE_LOG(LogTemp, Error, TEXT("ComponentTag is invalid tag in ClickInteractComponent")) }
	ComponentTags.Add(ComponentTag.GetTagName());
}

//////////////////////////////////////////////// Collision /////////////////////////////////////////////////////////////

void UClickInteractComponent::InitComponentRange()
{
	//Basic
	ComponentRange = CreateDefaultSubobject<UBoxComponent>(TEXT("ComponentRange"));
	ComponentRange->SetupAttachment(this);
	
	ComponentRange->SetBoxExtent(FVector(64.0,64.0,64.0));
	ComponentRange->PrimaryComponentTick.bCanEverTick = false;
	ComponentRange->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	ComponentRange->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ComponentRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	ComponentRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UClickInteractComponent::InitClickArea()
{
	//Basic
	ClickArea = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickArea"));
	ClickArea->SetupAttachment(this);
	
	ClickArea->PrimaryComponentTick.bCanEverTick = false;
	ClickArea->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	ClickArea->ShapeColor = FColor::Green;

	ClickArea->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ClickArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	ClickArea->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void UClickInteractComponent::OnComponentRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActorsInRange.Add(OtherActor);
	SetWidgetVisibility(true);
}

void UClickInteractComponent::OnComponentRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorsInRange.Remove(OtherActor);
	SetWidgetVisibility(false);
}

bool UClickInteractComponent::TryInteractWith(const AActor* InteractingCharacter)
{
	if(!InteractingCharacter) { return false; }
	if (!InteractActions.IsValid()) { return false; }
	UInteractActions* Actions = UInteractActionsManager::GetInteractActions(InteractActions);

	if (InteractionType.IsValid()){ return false; }
	
	Actions->PerformInteraction(InteractingCharacter, GetOwner(), InteractionType);
	
	return true;
}

void UClickInteractComponent::TryInteractWith_I_Implementation(const AActor* InteractingCharacter)
{
	//IClickInteractComponent_I::TryInteractWith_I_Implementation(OwningActor, InteractionTarget);
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
