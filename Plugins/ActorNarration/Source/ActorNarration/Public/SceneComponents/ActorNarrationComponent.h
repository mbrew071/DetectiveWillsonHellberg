// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Components/SceneComponent.h"
#include "Enumerations/NarrationLineType.h"
#include "Structures/NarrationLine.h"
#include "ActorNarrationComponent.generated.h"


class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrationEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNarrationDisrupt, FName ,NarrationName);

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

	//TODO Store this in a singleton 
	//Data table that contains Narration text styles in this project
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* TextSytles;

	//TODO Store this in a singleton
	//Data table that contains Narration text anim styles in this project
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* AnimStyles;
	
	//When other than "None" a narration is currently being played.
	UPROPERTY()
	FName OngoingNarration = "None";

	FTimerHandle TimerHandle;
	
	/**
	* Starts playing Narration Line. If other narration is currently ongoing it will be replaced with this new one.
	* When ongoing narration is replaced by other OnNarrationDisrupt is called
	* When Narration begins OnNarrationBegin is called,
	* when Narration ends OnNarrationEnd is called.
	*
	* @param NarrationLine	Narration that will be played
	* @param NarrationName	Must not be "None"
	*/
	UFUNCTION(BlueprintCallable, Category="Narration")
	void PlayNarration(FNarrationLine NarrationLine, FName NarrationName = "DefaultName");

private:
	//Figures out and returns the type of provided NarrationLine
	static ENarrationLineType GetNarrationLineType(const FNarrationLine& NarrationLine);

	void HandleNarrationBoth(const FNarrationLine& NarrationLine, FName& NarrationName);

	void HandleNarrationTextOnly(const FNarrationLine& NarrationLine, FName& NarrationName);
	
	void HandleNarrationAudioOnly(const FNarrationLine& NarrationLine, FName& NarrationName);

	void HandleNarrationEmpty();

	//Called when Narration begins
	void BeginNarration(FName NarrationName);

	//Called when Narration stops
	void EndNarration();

	void NarrationLoop(const FNarrationLine& NarrationLine);
	
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
