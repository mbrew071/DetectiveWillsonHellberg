// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SettingsUIController_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USettingsUIController_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API ISettingsUIController_I
{
	GENERATED_BODY()


	//////////////// Popup Backend:
public:

	/**
	* This will apply graphics settings, and cause the Confirm popup to spawn.
	*
	* Implemented on Backend
	* 
	*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void TryApplySettings(const int32 ConfirmTime = 7 ); 

	/*
	 * This will confirm applied graphics settings.
	 *
	 * Implemented on Backend
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void ConfirmApplyGraphicsSettings(); 

	/*
	 * This will Cancel applied graphics settings.
	 *
	 * Implemented on Backend
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Popup")
	void CancelApplyGraphicsSettings(); 

};
