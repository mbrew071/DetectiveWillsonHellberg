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
class UWidgetComponent;
class UBoxComponent;
class UUserWidget;

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

	//Tags for collision components that will be used as Range area
	//eg. "SceneComponent.ClickInteract.RangeArea"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag RangeAreaTag = FGameplayTag::RequestGameplayTag(FName("SceneComponent.ClickInteract.RangeArea"));
	
	//Tag for the type of interaction that this component has
	//eg. "InteractionType.Pickup"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag InteractionType = FGameplayTag::RequestGameplayTag(FName("InteractionType.Pickup"));

	//Only characters with this tag can interact with this component
	//eg. "Actor.Character.Player"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Tags")
	FGameplayTag OverlapTarget = FGameplayTag::RequestGameplayTag(FName("Actor.Character.Player"));

private:	
	void InitComponentTags();
	
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
	//Shapes that define the range area
	UPROPERTY()
	TArray<UActorComponent*> RangeArea;

private:
	void InitRangeArea();
	
	//Adds actor that is close enough to interact
	UFUNCTION()
	void OnComponentRangeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);
	// Removes actor as in is no longer in close enough to interact 
	UFUNCTION()
	void OnComponentRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void RangeAreaAddDynamic();

	// It updates visibility of widget, Called when character enters or leaves Range Collision
	void UpdateWidgetVisibility();

	/////////////////////////////////////////// Widget //////////////////////////////////////////////////////////

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	UWidgetComponent* WidgetComponent;

	//TODO STORE IT IN SINGLETON!, Here store only the override
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

	//If we are using no distance check then init widget visibility
	void SetNoDistanceVisibility();
	
	/////////////////////////////////////////// Interaction //////////////////////////////////////////////////////////
public:
	//TODO STORE IT IN SINGLETON!
	//Class that contains code for interaction behaviour.
	//When player interacted with sth the action is coded there.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Interaction")
	TSoftClassPtr<UInteractActions> InteractActions; 

private:
	//Executed when Player clicked on an instance of this component
	UFUNCTION(BlueprintCallable, Category="ClickInteract")
	void TryInteractWith(const AActor* InteractingCharacter);

public:
	virtual void TryInteractWith_I_Implementation(const AActor* InteractingCharacter) override;

	//////////////////////////////////////// Delegates ///////////////////////////////////////////////////////////

};



/*	Setup:
 *	0. Player controller -> Set "EnableClickEvents" to true (without this I wont be able to click on collision)
 *	1. Add this component to an actor
 *	2. Set Tags
 *	3. Setup ClickArea Collision ObjectType (to eg. "Interactable", Create new collision channel if needed)
 *	4. Setup RangeArea Collision to overlap Character (eg. ignore everything, overlap Pawn)
 *	5. Set WidgetClass (if I want a widget to spawn when character is in range)
 *	6. Set Interaction class. (Each project should have a custom interaction class)
 */
