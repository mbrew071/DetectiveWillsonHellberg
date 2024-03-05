// All rights reserved hehe

#pragma once

#include "CoreMinimal.h"
#include "Enumerations/NarrationLineType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorNarrationController.generated.h"

//Singleton designed to control the ActorNarration components in the world
UCLASS()
class ACTORNARRATION_API UActorNarrationController : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Decides what kind of narrations are allowed to be played.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Design|Controls")
	TEnumAsByte<ENarrationLineType>	NarrationLineType = Both;

	//Widget class that will be displayed when an actor starts narration
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Presets")
	TSoftObjectPtr<USoundAttenuation> SoundAttenuation;
	
	/*
	 *	DATA TABLES 
	 */
	
	//Data table that contains Narration text styles in this project
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Presets")
	TSoftObjectPtr<UDataTable> TextStylePresets;
	
	//Data table that contains Narration text anim styles in this project
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Presets")
	TSoftObjectPtr<UDataTable> NarrationDelaysPreset;
	
	//Data table that contains Data Tables of struct FNarrationLine
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|Presets")
	TSoftObjectPtr<UDataTable> MasterNarrationDT;
	
	UFUNCTION(BlueprintCallable, Category="Initialization")
	bool InitializePlugin(TEnumAsByte<ENarrationLineType> InNarrationLineType,  TSoftObjectPtr<UDataTable> InTextStylePresets,  TSoftObjectPtr<UDataTable> InTextAnimPresets,  TSoftObjectPtr<UDataTable> InMasterNarrationDT, TSoftClassPtr<UUserWidget> InWidgetClass, TSoftObjectPtr<USoundAttenuation> InSoundAttenuation);
};
