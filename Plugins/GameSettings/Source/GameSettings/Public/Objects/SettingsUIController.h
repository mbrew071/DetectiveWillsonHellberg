// All rights reserved hehe

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SettingsUIController_I.h"
#include "SettingsUIController.generated.h"

class UGameSettingsController;
/**
 * This is the base class for any settings controller
 * 
 * This class contains functions that can be used only in backend of GameSettings
 * If a function is meant to be used both by backend and frontend please add it to "UGameSettingsHelpers"
 */
UCLASS(Blueprintable)
class GAMESETTINGS_API USettingsUIController : public UObject, public ISettingsUIController_I
{
	GENERATED_BODY()

public:
	//Called on object creation, by the controller subsystem
	UFUNCTION(BlueprintNativeEvent, Category="GameSettings")
	void CustomConstruction(); 

	//Points to the Main GameInstanceSubsystem 
	UPROPERTY(BlueprintReadOnly,Category="xD")
	UGameSettingsController* GameSettingsController; 
	
	//Widget that spawned this settings object by subsystem
	UPROPERTY(BlueprintReadOnly, Category="GameSettings")
	const UObject* Owner; 
};

