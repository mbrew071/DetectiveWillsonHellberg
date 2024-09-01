// All rights reserved hehe

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enumerations/ScreenResolutionEnum.h"
#include "Structures/GraphicsSettings.h"
#include "GameSettingsHelpers.generated.h"

//This class contains functions that can be used both in frontend and backend of gamesettings
UCLASS()
class GAMESETTINGS_API UGameSettingsHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	////////////////////////*SCREEN RESOLUTION*/
	
public:
	// Converts FIntPoint struct to Screen resolution enum 
	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool EnumToResolution(const EScreenResolutionEnum InScreenResolutionEnum, FIntPoint& OutRes); 

	// Converts Screen resolution enum to FIntPoint struct
	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool ResolutionToEnum(const FIntPoint InRes, EScreenResolutionEnum& OutScreenResolutionEnum); 

	// Converts int32 to resolution enum 
	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool IntToResolutionEnum(const int32 Index, EScreenResolutionEnum& OutScreenResolutionEnum); 

	// Convert resolution String to resolution enum
	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool ResolutionStringToEnum(const FString StringResolutionEnum, EScreenResolutionEnum& OutScreenResolutionEnum); 

	/////////////////////////////*WINDOW MODE*/
	
	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool WindowModeStringToEnum(const FString& WindowModeString, TEnumAsByte<EWindowMode::Type>& OutWindowMode); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Enums")
	static bool IntToWindowModeEnum(const int32& WindowModeInt, TEnumAsByte<EWindowMode::Type>& OutWindowMode); 

	////////////////////////////*OTHER*/
	
	UFUNCTION()
	static bool GetGraphicsSettingsFromEngine(FGraphicsSettings& GraphicSettingsCombined);


	//If Tag is "GameSettings" and index is 0 it will return "GameSettings"
	//If Tag is "GameSettings.Input" and index is 0 it will return "GameSettings"
	//If Tag is "GameSettings.Input" and index is 1 it will return "GameSettings.Input"
	UFUNCTION(BlueprintCallable, Category="GameSettings|GameplayTag")
	static bool SplitGameplayTag(const FGameplayTag& GameplayTag, FGameplayTag& OutGameplayTag, int32 PartIndex = 1); 
	
	//If Tag is invalid it will return -1
	//If Tag is "GameSettings" it will return 0
	//If Tag is "GameSettings.Input" it will return 1
	//If Tag is "GameSettings.Input.Movement" it will return 2
	UFUNCTION(BlueprintCallable, Category="GameSettings|GameplayTag", BlueprintPure)
	static void GetLastPartIndex(const FGameplayTag GameplayTag, int32& LastPartIndex);
};
