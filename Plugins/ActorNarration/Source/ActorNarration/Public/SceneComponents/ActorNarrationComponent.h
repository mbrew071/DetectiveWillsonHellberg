// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Enumerations/NarrationLineType.h"
#include "Structures/NarrationLine.h"
#include "ActorNarrationComponent.generated.h"


class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationDisrupt);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTORNARRATION_API UActorNarrationComponent : public USceneComponent
{
	GENERATED_BODY()

	/////////////////////////////////////// GENERAL ///////////////////////////////////////////////////////
public:	
	// Sets default values for this component's properties
	UActorNarrationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	///////////////////////////////////// WIDGET /////////////////////////////////////////////////////////////
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	UWidgetComponent* WidgetComponent;

	//Widget class that will be displayed when a character is within the interaction range
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;

	///////////////////////////////////// Narration ///////////////////////////////////////////////////////////////
public:

	//When true a narration is currently being played
	UPROPERTY()
	bool bNarrationOngoing = false;
	
	//Starts playing Narration Line.
	//When it begins OnLineBegin is called, when it ends OnLineEnd is called.
	UFUNCTION(BlueprintCallable, Category="Narration")
	void PlayNarration(FNarrationLine NarrationLine);

private:
	//Figures out and returns the type of provided NarrationLine
	static ENarrationLineType GetNarrationLineType(const FNarrationLine& NarrationLine);

	void HandleNarrationBoth(const FNarrationLine& NarrationLine);

	void HandleNarrationTextOnly(const FNarrationLine& NarrationLine);
	
	void HandleNarrationAudioOnly(const FNarrationLine& NarrationLine);

	void HandleNarrationEmpty();

	//Called when Narration begins
	void BeginNarration();

	//Called when Narration stops
	void EndNarration();
	
public:	
	//It stops currently playing narration line.
	//When this function finishes the OnLineDisrupt is called
	UFUNCTION(BlueprintCallable, Category="Narration")
	void DisruptNarration();
	
	///////////////////////////////////// Delegates ///////////////////////////////////////////////////////////////
public:
	//Called when narration begins
	UPROPERTY(BlueprintAssignable, Category="Binds")
	FOnNarrationBegin OnNarrationBegin;

	//Called when narration successfully ends 
	UPROPERTY(BlueprintAssignable, Category="Binds")
	FOnNarrationEnd OnNarrationEnd;

	//Called when narration failed to finish because it was disrupted.
	UPROPERTY(BlueprintAssignable, Category="Binds")
	FOnNarrationDisrupt OnNarrationDisrupt;
};
