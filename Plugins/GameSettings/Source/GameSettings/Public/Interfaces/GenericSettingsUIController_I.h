// All rights reserved KAG 2024

#pragma once

#include <GameplayTagContainer.h>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GenericSettingsUIController_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGenericSettingsUIController_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API IGenericSettingsUIController_I
{
	GENERATED_BODY()

public:
	/**
	* Use this to set setting inside UI controller
	*
	* @param GameplayTag use it to specify the setting you want to set
	*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|GenericSettings")
	void SetGenericSettingInt(const FGameplayTag GameplayTag, const int32 Value);  

	/**
	* Use this to set setting inside UI controller
	*
	* @param GameplayTag use it to specify the setting you want to set
	*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|GenericSettings")
	void SetGenericSettingBool(const FGameplayTag GameplayTag, const bool Value); 

	/**
	* Use this to set setting inside UI controller
	*
	* @param GameplayTag use it to specify the setting you want to set
	*/
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="GameSettings|GenericSettings")
	void SetGenericFloatSetting(const FGameplayTag GameplayTag, const float Value);
	
};
