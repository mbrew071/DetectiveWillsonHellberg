// All rights reserved, HelloIT Norweskie


#include "SceneComponents/ClickInteractComponent.h"

#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Objects/InteractActions.h"
#include "Objects/InteractActionsManager.h"

//////////////////////////////////////////////// General /////////////////////////////////////////////////////////////
UClickInteractComponent::UClickInteractComponent()
{
	//Idk This doesnt work, no idea how to default it to a blueprint widget class
	//WidgetClass = TSoftClassPtr<UUserWidget>(FString::Printf(TEXT("%s/Plugins/ClickInteractionSystem/Content/WBP_InteractPin.uasset"), *FPaths::ProjectContentDir()));

	InitClickInteractComponent();
	InitSphereComponentCollision();
	InitSphereComponentClick();
	InitWidgetComponent();
}

void UClickInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTags();

	//TODO try do these on construction
	SphereComponentRange->OnComponentBeginOverlap.AddDynamic(this, &UClickInteractComponent::OnSphereCollisionBeginOverlap);
	SphereComponentRange->OnComponentEndOverlap.AddDynamic(this, &UClickInteractComponent::OnSphereCollisionEndOverlap);
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

void UClickInteractComponent::InitSphereComponentCollision()
{
	//Basic
	SphereComponentRange = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentCollision"));
	SphereComponentRange->SetupAttachment(this);

	SphereComponentRange->SetSphereRadius(RadiusSphereComponentRange, false);
	
	SphereComponentRange->PrimaryComponentTick.bCanEverTick = false;
	SphereComponentRange->PrimaryComponentTick.bStartWithTickEnabled = false;

	
	SphereComponentRange->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SphereComponentRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponentRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UClickInteractComponent::InitSphereComponentClick()
{
	//Basic
	SphereComponentClick = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponentClick"));
	SphereComponentClick->SetupAttachment(this);

	SphereComponentClick->SetSphereRadius(RadiusSphereComponentClick, false);
	
	SphereComponentClick->PrimaryComponentTick.bCanEverTick = false;
	SphereComponentClick->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SphereComponentClick->ShapeColor = FColor::Green;

	SphereComponentClick->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SphereComponentClick->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponentClick->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void UClickInteractComponent::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ActorsInRange.Add(OtherActor);
	SetWidgetVisibility(true);
}

void UClickInteractComponent::OnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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
