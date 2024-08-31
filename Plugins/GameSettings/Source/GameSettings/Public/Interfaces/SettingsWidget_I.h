// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SettingsWidget_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USettingsWidget_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API ISettingsWidget_I
{
	GENERATED_BODY()

	
public:
	//This is called whenever the graphics settings have changed
	//Use this to update visuals of frontend  buttons 
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|GenericSettings")
	void CurrentGenericSettingsChanged(const TMap<FGameplayTag, bool>& BoolSettings, const TMap<FGameplayTag, int32>& IntSettings, const TMap<FGameplayTag, float>& FloatSettings);  

	//This is called whenever the input settings have changed
	//Use this to update visuals of frontend  buttons 
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Input")
	void CurrentInputSettingsChanged( const TMap<FGameplayTag, FInputChord>& OutKeybindingSettings);  

/////////////// Popup Frontend	
	
	/*
	 * This will spawn the popup.
	 *
	 * Implemented on Frontend
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void SpawnConfirmPopup(); 

	/*
	 * This will despawn the popup.
	 *
	 * Implemented on Frontend
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void DespawnConfirmPopup(); 
	
	/*
	 * This will send info about change of remaining time in settings confirmation popup.
	 *
	 * Implemented on Frontend
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void OnConfirmPopupTimeChange(const int32 NewTime); 

};
