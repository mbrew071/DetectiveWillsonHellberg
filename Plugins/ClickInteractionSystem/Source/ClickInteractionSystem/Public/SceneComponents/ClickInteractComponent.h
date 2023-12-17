// All rights reserved, HelloIT Norweskie

/*
 * We add this component to an actor with whom we want to interact with by clicking
 * This system requires the player controller to have enabled "EnableClickEvents"
 * This system requires gameplay tags 
 */

#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"
#include "Interfaces/ClickInteractComponent_I.h"
#include "ClickInteractComponent.generated.h"

class UInteractActions;
class USphereComponent;
class UWidgetComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, Blueprintable))
class CLICKINTERACTIONSYSTEM_API UClickInteractComponent : public USceneComponent, public IClickInteractComponent_I
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClickInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	
	void InitClickInteractComponent();
	//////////////////////////////////////////// Tags /////////////////////////////////////////////////////////////////

public:
	//Tag of this component.
	//eg. "SceneComponent.ClickInteract"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag ComponentTag = FGameplayTag::RequestGameplayTag(FName("SceneComponent.ClickInteract"));

	//Tag for the type of interaction that this component has
	//eg. "InteractionType.Pickup"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag InteractionType = FGameplayTag::RequestGameplayTag(FName("InteractionType.Pickup"));

	//Only characters with this tag can interact with this component
	//eg. "Actor.Character.Player"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag OverlapTarget = FGameplayTag::RequestGameplayTag(FName("Actor.Character.Player"));

private:	
	void InitTags();
	
	////////////////////////////////////////////Collision//////////////////////////////////////////////////////////////////
public:
	//When true only Characters close to the component will be able to interact
	//When false Character can interact while being far from this component 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|InteractCollision")
	bool bCheckDistance = true;
	
	//Actors that are in range to interact with the owner of this component
	UPROPERTY(BlueprintReadWrite, Category="Design|InteractCollision")
	TArray<AActor*> ActorsInRange;
	
public:
	//This Sphere checks if the character is near enough to interact with owner of this component
	//Only Characters overlapped by this collision can interact by clicking on click collision
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|InteractCollision")
	USphereComponent* SphereComponentRange;

	//Initial Radius of the interaction distance area
	UPROPERTY(BlueprintReadWrite, Category="Design|InteractCollision")
	float RadiusSphereComponentRange = 100.0f;

private:
	void InitSphereComponentCollision();

public:
	//Collision which will catch clicks
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|InteractCollision")
	USphereComponent* SphereComponentClick;

	//Initial Radius of the clickable area
	UPROPERTY(BlueprintReadWrite, Category="Design|InteractCollision")
	float RadiusSphereComponentClick = 20.0f;

private:
	void InitSphereComponentClick();

	//Adds actor that is close enough to interact
	UFUNCTION()
	void OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);
	// Removes actor as in is no longer in close enough to interact 
	UFUNCTION()
	void OnSphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/////////////////////////////////////////// Widget //////////////////////////////////////////////////////////

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	UWidgetComponent* WidgetComponent;

	//Widget class that will be displayed when a character is within the interaction range
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;
	
	//Ref to spawned UserWidget
	UPROPERTY()
	UUserWidget* UserWidget;
	
private:
	void InitWidgetComponent();

	//Shows or hides the widget that indicates that Character is close enough to interact
	UFUNCTION(BlueprintCallable, Category="Design|Widget")
	bool SetWidgetVisibility (const bool bNewVisibility);
	
	/////////////////////////////////////////// Interaction //////////////////////////////////////////////////////////
public:
	//Class that contains code for interaction behaviour.
	//When player interacted with sth the action is coded there.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Interaction")
	TSoftClassPtr<UInteractActions> InteractActions; 

private:
	//Executed when Player clicked on an instance of this component
	UFUNCTION(BlueprintCallable)
	bool TryInteractWith(const AActor* InteractingCharacter);

public:
	virtual void TryInteractWith_I_Implementation(const AActor* InteractingCharacter) override;
	
};

/*	Setup:
 *	0. Player controller -> Set "EnableClickEvents" to true (without this I wont be able to click on collision)
 *	1. Add this component to an actor
 *	2. Set Tags
 *	3. Set ClickCollision Collision ObjectType (to eg. "Interactable", Create new collision channel if needed)
 *	4. Set RangeCollision Collision to overlap Character (eg. ignore everything, overlap Pawn)
 *	5. Set WidgetClass (if I want a widget to spawn when character is in range)
 *	6. Set Interaction class. (Each project should have a custom interaction class)
 */
