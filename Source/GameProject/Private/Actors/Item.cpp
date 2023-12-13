// All rights reserved, HelloIT Norweskie


#include "Actors/Item.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	InitComponents();
	InitCollision();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//Start with disabled visibility
//	SetWidgetVisibility(false);
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
	
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(SceneComponent);
	Widget->PrimaryComponentTick.bCanEverTick = false;
	Widget->PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);
	SphereComponent->PrimaryComponentTick.bCanEverTick = false;
	SphereComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void AItem::InitCollision()
{
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	SphereComponent->CanCharacterStepUpOn = ECB_No;
}

void AItem::SetWidgetVisibility(const bool bNewVisibility)
{
	//TODO  Fix me 
	Widget->SetVisibility(bNewVisibility);
	Widget->SetHiddenInGame(!bNewVisibility);
}


