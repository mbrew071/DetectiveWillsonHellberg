// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "GraphNarrationDelegates.h"
#include "GraphNarrationTarget.generated.h"

class UNarrationGraph;
class UWidgetComponent;

//TODO rename this to GraphNarrationTarget

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLENARRATION_API UGraphNarrationTarget : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGraphNarrationTarget();
	UPROPERTY()
	UWidgetComponent* WidgetComponent;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UNarrationGraph* CurrentGraph;

public:	
	UPROPERTY(EditDefaultsOnly, Category="SimpleNattation")
	TSoftClassPtr<UNarrationGraph> DefaultNarrationGraph;
	
	/**
	 * @param NarrationActors List of actors that participate in dialogue
	 * @param GraphClass Narration graph to play. If none then DefaultNarrationGraph will be played. 
	 */
	UFUNCTION(BlueprintCallable, Category="SimpleNarration")
	bool BeginNarrationGraph(TArray<AActor*> NarrationActors, TSoftClassPtr<UNarrationGraph> GraphClass, FOnGraphNarrationEnd OnGraphEnd);

	//This stops current dialogue from playing 
	UFUNCTION(BlueprintCallable, Category="SimpleNarration")
	void EndNarrationGraph(const FGameplayTag Reason);
private:
	//TODO add it to Player screen and not the Viewport
	bool CreateNarrationGraphObject(UNarrationGraph*& OutGraph, TSoftClassPtr<UNarrationGraph>& Class);
};
