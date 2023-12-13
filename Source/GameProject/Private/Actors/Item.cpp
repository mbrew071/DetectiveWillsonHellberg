// All rights reserved, HelloIT Norweskie


#include "Actors/Item.h"

#include "GameplayTagContainer.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	InitComponents();
	InitCollision();
	InitTags();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Start with disabled widget
	SetWidgetVisibility(false);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AItem::InitComponents()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	RootComponent->PrimaryComponentTick.bCanEverTick = false;
	RootComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComponent->SetupAttachment(SceneComponent);
	WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	WidgetComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);
	SphereComponent->PrimaryComponentTick.bCanEverTick = false;
	SphereComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void AItem::InitTags()
{
	Tags.Add(FGameplayTag::RequestGameplayTag(FName("Actor.Interactable.Item")).GetTagName());
}

void AItem::InitCollision()
{
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComponent->CanCharacterStepUpOn = ECB_No;
}

bool AItem::SetWidgetVisibility(const bool bNewVisibility)
{
	if (!WidgetComponent) { return false; }
	if (!bNewVisibility)
	{
		//remove current widget 
		if (UserWidget) {UserWidget->RemoveFromParent();}
		if (UserWidget) {UserWidget->Destruct();}

		//outro
		WidgetComponent->SetWidget(nullptr);
		WidgetComponent->SetVisibility(bNewVisibility);
		WidgetComponent->SetHiddenInGame(!bNewVisibility);
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
		UserWidget->AddToViewport();
		WidgetComponent->SetWidget(UserWidget);
		WidgetComponent->SetVisibility(bNewVisibility);
		WidgetComponent->SetHiddenInGame(!bNewVisibility);
		return true;
	}
	//Spawn failed therefore we cleanup
	WidgetComponent->SetWidget(nullptr);
	WidgetComponent->SetVisibility(bNewVisibility);
	WidgetComponent->SetHiddenInGame(!bNewVisibility);
	return false;
}

void AItem::SetWidgetVisibility_I_Implementation(const bool bNewVisibility)
{
	//IItem_I::SetWidgetVisibility_I_Implementation(bNewVisibility);
	SetWidgetVisibility(bNewVisibility);
}


