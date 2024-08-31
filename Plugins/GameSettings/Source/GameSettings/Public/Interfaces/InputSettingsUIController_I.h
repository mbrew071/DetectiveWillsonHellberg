// All rights reserved KAG 2024

#pragma once

#include <GameplayTagContainer.h>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Framework/Commands/InputChord.h"
#include "InputSettingsUIController_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputSettingsUIController_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API IInputSettingsUIController_I
{
	GENERATED_BODY()

public:
	
	/**
	 * This sets input of action specified by @param GameplayTag
	 *
	 * @param Value the input chord of the action
	 * @param GameplayTag use it to specify the setting you want to set
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|Input")
	void SetInputSetting(const FGameplayTag GameplayTag, const FInputChord Value);  

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameSettings|Input")
	void ApplyInputSettings(const TMap<FGameplayTag, FInputChord>& InputSettings);  

};
