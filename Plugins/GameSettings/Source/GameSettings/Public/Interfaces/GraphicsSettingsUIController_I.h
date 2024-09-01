// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GraphicsSettingsUIController_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGraphicsSettingsUIController_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMESETTINGS_API IGraphicsSettingsUIController_I
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="GameSettings")
	void ApplyGraphicsSettings(const TMap<FGameplayTag, bool>& BoolSettings, const TMap<FGameplayTag, int32>& IntSettings, const TMap<FGameplayTag, float>& FloatSettings); 

};
