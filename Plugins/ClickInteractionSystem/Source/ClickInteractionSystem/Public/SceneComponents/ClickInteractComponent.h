// All rights reserved, KAG 2024

/*
 * We add this component to an actor with whom we want to interact with by clicking
 * This system requires the player controller to have enabled "EnableClickEvents"
 * This system requires gameplay tags 
 */

#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"
#include "ClickInteractDelegates.h"
#include "Interfaces/ClickInteractComponent_I.h"
#include "Engine/StreamableManager.h"
#include "ClickInteractComponent.generated.h"

class UInteractActions;
class UWidgetComponent;
class UBoxComponent;
class UUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, Blueprintable), HideCategories=(Replication, Navigation, AssetUserData, LOD, Cooking, Activation, ComponentReplication, ComponentTick, Socket, Rendering, Physics))
class CLICKINTERACTIONSYSTEM_API UClickInteractComponent : public USceneComponent, public IClickInteractComponent_I
{
	GENERATED_BODY()

	//////////////////////////////////////////////// Construction  //////////////////////////////////////////////
public:	
	UClickInteractComponent();
	
	//////////////////////////////////////////////// BeginPlay  ////////////////////////////////////////////////
protected:
	virtual void BeginPlay() override;
	
private:
	void InitRangeArea();
	void RangeAreaAddDynamic();
	//If we are using no distance check then init widget visibility
	void SetNoDistanceVisibility();

	//////////////////////////////////////////////// RangeArea Overlaps //////////////////////////////////////////
private:
	//When true only Characters close to the component will be able to interact
	//When false Character can interact while being far from this component 
	bool ShouldCheckRange();
	
	//Adds actor that is close enough to interact
	UFUNCTION()
	void OnComponentRangeAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Removes actor as in is no longer in close enough to interact
	UFUNCTION()
	void OnComponentRangeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//////////////////////////////////////////////// Widget Visibility ////////////////////////////////////////
private:
	// It updates visibility of widget, Called when character enters or leaves Range Collision
	
	void ShowWidget();
	TSharedPtr<FStreamableHandle> WidgetLoadHandle;
	void OnWidgetClassLoaded();
	void HideWidget();
	
	/////////////////////////////////////////// Interaction //////////////////////////////////////////////////////
public:
	//Executed when Player clicked on ClickArea  
	virtual void TryInteractWith_I_Implementation(AActor* InteractingActor) override;
	
private:

	UFUNCTION(BlueprintCallable, Category="SceneComponent.ClickInteract")
	void TryInteractWith(AActor* InteractingActor);
	
	//////////////////////////////////////////// Variables /////////////////////////////////////////////////////
public:
	/*//Tag for the type of interaction that this component has
	//eg. "InteractionType.Pickup"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design")
	FGameplayTag InteractionType = FGameplayTag::RequestGameplayTag(FName("InteractionType.Pickup")); */
	
	//Actors that are currently close enough to interact this component
	UPROPERTY(BlueprintReadOnly, Category="SceneComponent.ClickInteract")
	TArray<AActor*> ActorsInRange;

	UPROPERTY(BlueprintReadOnly, Category="SceneComponent.ClickInteract")
	bool bInteractionOngoing = false;

public:
	//Shapes that define the range area
	UPROPERTY()
	TArray<UActorComponent*> RangeAreaComponents;

	UPROPERTY(BlueprintAssignable)
	FOnInteractionBegin OnInteractionBegin;
	
	UFUNCTION()
	void OnInteractionActionEnd(const bool bSuccess);
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInteractionEnd OnInteractionEnd;
	
	/////////////////////////////////////////// Widget ///////////////////////////////////////////////////////
public:	
	//Widget class that will be displayed when a character is within the interaction range
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design")
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY()
	UWidgetComponent* WidgetComponent;
};

