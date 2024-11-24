// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "ClickInteractController.generated.h"

//class UInteractActions;
class UUserWidget;

UCLASS()
class CLICKINTERACTIONSYSTEM_API UClickInteractController : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="ClickInteract")
	bool InitializePlugin( TSoftClassPtr<UUserWidget> InDefaultWidget, FGameplayTag InRangeArea, FGameplayTag InOverlapTarget); 

public:
	//Class that contains code for interaction behaviour.
	//When player interacted with sth the action is coded there.
	/*
	UPROPERTY(BlueprintReadOnly, Category="ClickInteract")
	TSoftClassPtr<UInteractActions> InteractActions;
	*/

	//This widget will be shown when interaction is possible AND if a component doesnt override it with a custom widget 
	UPROPERTY(BlueprintReadOnly, Category="ClickInteract")
	TSoftClassPtr<UUserWidget> DefaultWidget;

	/*//Tag of all InteractComponents
	//eg. "SceneComponent.ClickInteract"
	UPROPERTY(BlueprintReadOnly, Category="ClickInteract")
	FGameplayTag InteractComponent;*/

	//Tag of collision components that will be used as Range area
	//eg. "SceneComponent.ClickInteract.RangeAreaComponents"
	UPROPERTY(BlueprintReadOnly, Category="ClickInteract")
	FGameplayTag RangeArea;

	//Only characters with this tag can interact with InteractComponents
	//eg. "Actor.Character.Player"
	UPROPERTY(BlueprintReadOnly, Category="ClickInteract")
	FGameplayTag OverlapTarget;
	
};

/*	Setup:
 *	0. Player controller -> Set "EnableClickEvents" to true (without this I wont be able to click on collision)
 *	1. Add InteractComponent to an actor
 *	
 *	2. Add, Setup ClickArea Collision -> ObjectType -> "Interactable" / (other if needed)
 *	3. Add, Setup RangeAreaComponents Collision -> (eg. ignore everything, overlap Pawn)
 *
 *	4. Set WidgetClass (if I want a widget to spawn when character is in range)
 *	5. Set interact tag
 */