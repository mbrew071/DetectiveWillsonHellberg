// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/GraphicsSettingsUIController_I.h"
#include "Objects/GenericSettingsUIController.h"
#include "GraphicsSettingsUIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMESETTINGS_API UGraphicsSettingsUIController : public UGenericSettingsUIController, public IGraphicsSettingsUIController_I
{
	GENERATED_BODY()
	
};
