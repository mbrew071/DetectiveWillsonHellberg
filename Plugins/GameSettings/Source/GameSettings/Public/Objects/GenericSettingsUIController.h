// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/GenericSettingsUIController_I.h"
#include "Objects/SettingsUIController.h"
#include "GenericSettingsUIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMESETTINGS_API UGenericSettingsUIController : public USettingsUIController, public IGenericSettingsUIController_I
{
	GENERATED_BODY()
	
};
