// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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

	//Starts playing Narration Line.
	//When it begins OnLineBegin is called, when it ends OnLineEnd is called.
	UFUNCTION(BlueprintCallable, Category="Narration")
	void PlayNarration(FNarrationLine NarrationLine);

	//Stops playing narration line.
	//When this function finishes the OnLineDisrupt is called
	UFUNCTION(BlueprintCallable, Category="Narration")
	void DisruptNarration(FNarrationLine NarrationLine);
	
	///////////////////////////////////// Delegates ///////////////////////////////////////////////////////////////

	//Called when narration begins
	UPROPERTY(BlueprintAssignable, Category="Narration|Binds")
	FOnNarrationBegin OnNarrationBegin;

	//Called when narration successfully ends 
	UPROPERTY(BlueprintAssignable, Category="Narration|Binds")
	FOnNarrationEnd OnNarrationEnd;

	//Called when narration failed to finish because it was disrupted.
	UPROPERTY(BlueprintAssignable, Category="Narration|Binds")
	FOnNarrationDisrupt OnNarrationDisrupt;
};
