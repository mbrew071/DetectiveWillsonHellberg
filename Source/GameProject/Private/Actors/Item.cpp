// All rights reserved, HelloIT Norweskie

#include "Actors/Item.h"
#include "GameplayTagContainer.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	InitSceneComponent();
	InitTags();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AItem::InitSceneComponent()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	RootComponent->PrimaryComponentTick.bCanEverTick = false;
	RootComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
}

void AItem::InitTags()
{
	Tags.Add(FGameplayTag::RequestGameplayTag(FName("Actor.Interactable.Item")).GetTagName());
}




