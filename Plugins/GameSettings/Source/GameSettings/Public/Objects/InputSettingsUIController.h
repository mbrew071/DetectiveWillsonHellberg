// All rights reserved KAG 2024

#pragma once

#include "CoreMinimal.h"
#include "Objects/SettingsUIController.h"
//#include "EnhancedInputSubsystems.h"
#include "Interfaces/InputSettingsUIController_I.h"
#include "InputSettingsUIController.generated.h"

class AController;

UCLASS()
class GAMESETTINGS_API UInputSettingsUIController : public USettingsUIController, public IInputSettingsUIController_I
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GameSettings|")
	AController* PlayerController;
};
