// All rights reserved KAG

#pragma once

#include "CoreMinimal.h"
#include "Structures/GenericSettings.h"
#include "Structures/InputChordSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettingsController.generated.h"

class UGraphicsSettingsUIController;
class USettingsUIController;
class UInputSettingsUIController;
/**
 * In game instance do:
 * 1 Init plugin
 *
 * In the right place call:
 * 2 SetConfirmedSettings  // to initialize the default settings of this game 
 *
 * Find IMC_Default ( or the one used in your project), and set up 
 */
UCLASS()
class GAMESETTINGS_API UGameSettingsController : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * This is used to set the default values for variables
	 *
	 * @param InJsonSerializationPath path where json with game settings will be. (only if InSaveToJson is true) 
	 * @param InJsonSerializedSettings List of settings tht wil lbe serialized to json
	 *
	 * @param InSettingCategories list of categories of settings. eg. for setting "GameSetting.Input.Movement.Forwards" the category will be "GameSetting.Input.Movement"
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	bool InitializePlugin(TSoftClassPtr<UGraphicsSettingsUIController> InGraphicsSettingsClass, TSoftClassPtr<USettingsUIController> InAudioSettingsClass, TSoftClassPtr<USettingsUIController> InGameSettingsClass, TSoftClassPtr<UInputSettingsUIController> InBindsSettingsClass, const TArray<FGameplayTag> InJsonSerializedSettings, FString InJsonSerializationPath, const TSet<FGameplayTag> InSaveGameSettings); 

///////// Initialize settings
	
	//Initializes default settings by combining them with json stored settings
	UFUNCTION(BlueprintCallable, Category="GameSettings")
	bool InitializeSettings(const TMap<FGameplayTag, bool>& DefaultBoolSettings,const TMap<FGameplayTag, int32>& DefaultIntSettings, const TMap<FGameplayTag, float>& DefaultFloatSettings, const TMap<FGameplayTag, FInputChord>& DefaultInputSettings); 
	
private:
	//UFUNCTION(BlueprintCallable)
	bool InitializeGenericSettings(const TMap<FGameplayTag, bool>& DefaultBoolSettings,const TMap<FGameplayTag, int32>& DefaultIntSettings, const TMap<FGameplayTag, float>& DefaultFloatSettings);

	//UFUNCTION(BlueprintCallable)
	bool InitializeInputSettings(const TMap<FGameplayTag, FInputChord>& DefaultInputSettings);

	//////////////////////// Default settings:
public:	
	/*
	//Attempts to find settings that were used when this game was played previously
	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	bool TryFindPreviousSettings(const TArray<FGameplayTag>& RequiredSettings, TMap<FGameplayTag, bool>& OutBoolSettings,TMap<FGameplayTag, int32>& OutIntSettings,TMap<FGameplayTag, float>& OutFloatSettings, TMap<FGameplayTag, FInputChord>& OutInputSettings);
	*/

	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	bool TryFindPreviousGenericSettings(const TArray<FGameplayTag>& RequiredSettings, TMap<FGameplayTag, bool>& OutBoolSettings,TMap<FGameplayTag, int32>& OutIntSettings,TMap<FGameplayTag, float>& OutFloatSettings); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	bool TryFindPreviousInputSettings(const TArray<FGameplayTag>& RequiredSettings, TMap<FGameplayTag, FInputChord>& OutInputSettings);
	
	///////////////////////////////////Default variables
private:
	UPROPERTY(EditAnywhere, Category="GameSettings|Controllers")
	TSoftClassPtr<UGraphicsSettingsUIController> GraphicsSettingsClass;

	UPROPERTY(EditAnywhere, Category="GameSettings|Controllers")
	TSoftClassPtr<USettingsUIController> AudioSettingsClass;

	UPROPERTY(EditAnywhere, Category="GameSettings|Controllers")
	TSoftClassPtr<USettingsUIController> GeneralSettingsClass;

	UPROPERTY(EditAnywhere, Category="GameSettings|Controllers")
	TSoftClassPtr<UInputSettingsUIController> BindsSettingsClass;
	
	//This array stores tags of settings that are loaded from json
	//If for example we do not want to allow the user to easily change some setting by modifying json
	//The game save system should take care of saving variables not stored in json such settings 
	UPROPERTY(EditAnywhere, Category="GameSettings|")
	TArray<FGameplayTag> JsonSerializedSettings;

	UPROPERTY(EditAnywhere, Category="GameSettings|")
	FString SettingsSavePath;

	//Settings that can be serialized
	UPROPERTY(EditAnywhere, Category="GameSettings|")
	TSet<FGameplayTag> SaveGameSettings;

public:
	/*UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GameSettings|")
	TArray<FGameplayTag> SettingCategories;*/
	
	//////////////////////////////////Variables that store currently confirmed game settings
private:
	UPROPERTY()
	TMap<FGameplayTag, bool> BoolSettings;
	
	UPROPERTY(SaveGame)
	TMap<FGameplayTag, bool> BoolSettingsSerializable;
	
	UPROPERTY()
	TMap<FGameplayTag, int32> IntSettings;

	UPROPERTY(SaveGame)
	TMap<FGameplayTag, int32> IntSettingsSerializable;

	UPROPERTY()
	TMap<FGameplayTag, float> FloatSettings;

	UPROPERTY(SaveGame)
	TMap<FGameplayTag, float> FloatSettingsSerializable;
	
	UPROPERTY()
	TMap<FGameplayTag, FInputChord> KeybindingSettings;
	
	/////////////////////////////////// Get UI Controller:
public:
	UFUNCTION(BlueprintCallable, Category="GameSettings|Get", meta=(WorldContext="WorldContextObject", CallableWithoutContext))
	bool GetGraphicSettingsController(const UObject* WorldContextObject, UObject*& OutGraphicsSettingsController);  

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get", meta=(WorldContext="WorldContextObject", CallableWithoutContext))
	bool GetAudioSettingsController(const UObject* WorldContextObject, UObject*& OutAudioSettingsController); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get", meta=(WorldContext="WorldContextObject", CallableWithoutContext))
	bool GetGeneralSettingsController(const UObject* WorldContextObject, UObject*& OutGeneralSettingsController); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get", meta=(WorldContext="WorldContextObject", CallableWithoutContext))
	bool GetBindsController(const UObject* WorldContextObject, UObject*& OutBindsSettingsController, AController* PlayerController);  
	
	//////////////////////////////////// Json Save / Load settings
private:

	//TODO REFACTOR THIS 
	bool SerializeInputTemp();

	//TODO REFACTOR THIS
	bool SerializeGenericTemp();
	
	//Serializes the struct to json and saves it at SavePath
	bool SerializeKeybindingSettings(const TMap<FGameplayTag, FInputChordSettings>& InKeybindingSettings, const FString& InFilePath, const FString InFileName = "KeybindingSettings.json");
	
	//Deserializes from json at FilePath and converts it to a struct
	bool DeserializeKeybindingSettings(TMap<FGameplayTag, FInputChordSettings>& OutKeybindings, const FString& InFilePath, const FString InFileName = "KeybindingSettings.json"); 

	//Serializes a GameSettings struct to json and saves it at SavePath
	bool SerializeGenericSettings(const TMap<FGameplayTag, FGenericSettings>& InGameSettings, const FString& InFilePath, const FString InFileName = "GenericSettings.json");

	//Deserializes from json at FilePath and converts it to a struct
	bool DeserializeGenericSettings(TMap<FGameplayTag, FGenericSettings>& OutGenericSettings, const FString& InFilePath, const FString InFileName = "GenericSettings.json");
	
	/////////////////////// Get settings:

public:
	//TODO rename to "GetAllSettings"
	//Function that returns the currently confirmed game settings
	UFUNCTION(BlueprintCallable, Category="GameSettings|")
	bool GetConfirmedSettings(TMap<FGameplayTag, bool>& OutBoolSettings,TMap<FGameplayTag, int32>& OutIntSettings, TMap<FGameplayTag, float>& OutFloatSettings,TMap<FGameplayTag,FInputChord>& OutKeybindingSettings);

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get")
	TMap<FGameplayTag, bool> GetBoolSettings( );

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get")
	TMap<FGameplayTag, int32> GetIntSettings();

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get")
	TMap<FGameplayTag, float> GetFloatSettings( );

	UFUNCTION(BlueprintCallable, Category="GameSettings|Get")
	TMap<FGameplayTag, FInputChord> GetInputSettings( );

	//////////////////////////// Set settings:

	//TODO rename to "SetAllSettings"
	//Function to confirm and save the applied settings
	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	bool SetConfirmedSettings(const TMap<FGameplayTag, bool> InBoolSettings, const TMap<FGameplayTag, int32> InIntSettings,const TMap<FGameplayTag, float> InfloatSettings, const TMap<FGameplayTag, FInputChord> InInputSettings); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	bool SetGenericSettings(const TMap<FGameplayTag, bool> InBoolSettings, const TMap<FGameplayTag, int32> InIntSettings,const TMap<FGameplayTag, float> InfloatSettings); 
	
	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	void SetBoolSettings(const TMap<FGameplayTag, bool> InBoolSettings); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	void SetIntSettings(const TMap<FGameplayTag, int32> InIntSettings); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	void SetFloatSettings(const TMap<FGameplayTag, float> InFloatSettings); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Set")
	void SetInputSettings(const TMap<FGameplayTag, FInputChord> InInputSettings); 

	////////////////// Get confirmed settings By TAG ( These are exposed to BPs )
protected:
	
	/*
	//TODO DEPRECIATE THIS FUNCTION reason this isn't very helpful
	/**
	* @param InSearchedTag searched tag
	* @param OutResult List of settings that match tags 
	* @return first found GenericSetting that matches searched searched tag
	#1#
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedGenericSettingByTag(const FGameplayTag InSearchedTag, TMap<FGameplayTag, FGenericSettings>& OutResult);

	//TODO DEPRECIATE THIS FUNCTION reason this isn't very helpful
	/**
	* @param InSearchedTag searched tag
	* @param OutResult map of settings that match tags 
	* @return success
	#1#
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedKeybindingSettingByTag(const FGameplayTag InSearchedTag, TMap<FGameplayTag, FInputChordSettings>& OutResult);
	*/
	
	/**
	* @param Tag searched tag
	* @param Setting first found setting that matches searched tag
	* @return True if setting was found
	*/
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedGenericBoolSetting(const FGameplayTag Tag, bool& Setting);

	/**
	* @param Tag searched tag
	* @param Setting first found setting that matches searched tag
	* @return True if setting was found
	*/
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedGenericIntSetting(const FGameplayTag Tag, int32& Setting);

	/**
	* @param Tag searched tag
	* @param Setting first found setting that matches searched tag
	* @return True if setting was found
	*/
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedGenericFloatSetting(const FGameplayTag Tag, float& Setting);

	/**
	* @param Tag searched tag
	* @param Setting first found setting that matches searched tag
	* @return True if setting was found
	*/
	UFUNCTION(BlueprintCallable, Category="GameSettings|GetConfirmed")
	bool GetConfirmedKeybingingSetting(const FGameplayTag Tag, FInputChord& Setting);

	/////////////////Add settings

	/*
	//Uniquely adds up generic settings,
	//If "A" already has a setting it wont be replaced by setting from "B"   
	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	void AddGenericSettingsUnique(const TMap<FGameplayTag, FGenericSettings> A, const TMap<FGameplayTag, FGenericSettings> B, TMap<FGameplayTag, FGenericSettings>& Result); 

	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	void AddGenericBoolSettingsUnique(const TMap<FGameplayTag, bool> A, const TMap<FGameplayTag, bool> B, TMap<FGameplayTag, bool>& Result); 
	*/
	
	/*
	//Uniquely adds up input settings
	//If "A" already has a setting it wont be replaced by setting from "B"
	UFUNCTION(BlueprintCallable, Category="GameSettings|Controllers")
	void AddInputSettingsUnique(const TMap<FGameplayTag, FInputChord>& A, const TMap<FGameplayTag, FInputChord>& B, TMap<FGameplayTag, FInputChord>& Result); 
	*/
	
	///////////////// Find Setting/Settings (Cpp Utility functions for this class)
private:
	
////////// Find Multiple settings: 
	//TODO move to library	
	/*
	/**
	* @param InSearchedTags List of searched tags of settings
	* @param InSource Source list 
	* @param OutResult a map of FGenericSettings that match InSearchedTags 
	* @return success
	#1#
	bool FindGenericSettingsByTag( const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, FGenericSettings>& InSource, TMap<FGameplayTag, FGenericSettings>& OutResult);
	*/

	/**
	* @param InSearchedTags List of searched tags of settings
	* @param InSource Source list 
	* @param OutResult a map of Bool settings that match InSearchedTags 
	* @return success
	*/
	bool FindBoolSettingsByTag(const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, bool>& InSource, TMap<FGameplayTag, bool>& OutResult);

	/**
	* @param InSearchedTags List of searched tags of settings
	* @param InSource Source list 
	* @param OutResult a map of int32 settings that match InSearchedTags 
	* @return success
	*/
	bool FindIntSettingsByTag(const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, int32>& InSource, TMap<FGameplayTag, int32>& OutResult);

	/**
	* @param InSearchedTags List of searched tags of settings
	* @param InSource Source list 
	* @param OutResult a map of float settings that match InSearchedTags 
	* @return success
	*/
	bool FindFloatSettingsByTag(const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, float>& InSource, TMap<FGameplayTag, float>& OutResult);

	
	/**
	* @param InSearchedTags List of searched tags of settings
	* @param InSource Source list 
	* @param OutResult map of FKeybindingSettings that match InSearchedTags
	* @return 
	*/
	bool FindKeybindingSettingsByTag(const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, FInputChord>& InSource, TMap<FGameplayTag, FInputChord>& OutResult);

///////// Find Array of settings setting:
	
	/*
	/**
	 * @param InSearchedTag searched tag
	 * @param InSource Source list 
	 * @param OutResult first found GenericSetting that matches searched searched tag
	 * @return success
	 #1#
	bool FindGenericSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, FGenericSettings>& InSource, TMap<FGameplayTag, FGenericSettings>& OutResult);
	*/
	
	/*
	/**
	* @param InSearchedTag searched tag
	* @param InSource Source list 
	* @param OutResult first found value of setting that matches searched tag
	* @return True when setting was found 
	#1#
	bool FindKeybindingSettingByTag(const FGameplayTag& InSearchedTag, const FInputChordSettings InSource, TMap<FGameplayTag, FInputChordSettings>& OutResult); 
	*/

////////// Find Single setting by type:
//TODO move to library
	
	/**
	* @param InSearchedTag searched tag
	* @param InSource Source list 
	* @param OutSetting first found setting that matches searched searched tag
	* @return True when setting was found
	*/
	UFUNCTION(BlueprintCallable, Category="GameSettings|FindSetting")
	bool FindGenericBoolSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, bool> InSource, bool& OutSetting);

	/**
	 * @param InSearchedTag searched tag
	 * @param InSource Source list 
	 * @param OutSetting first found setting that matches searched searched tag
	 * @return True when setting was found
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings|FindSetting")
	bool FindGenericIntSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, int32> InSource, int32& OutSetting);

	/**
	 * @param InSearchedTag searched tag
	 * @param InSource Source list 
	 * @param OutSetting first found setting that matches searched searched tag
	 * @return True when setting was found
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings|FindSetting")
	bool FindGenericFloatSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, float>& InSource, float& OutSetting);

	/**
	 * @param InSearchedTag searched tag
	 * @param InSource Source list 
	 * @param OutSetting first found setting that matches searched searched tag
	 * @return True when setting was found
	 */
	UFUNCTION(BlueprintCallable, Category="GameSettings|FindSetting")
	bool FindKeybindingSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, FInputChord>& InSource, FInputChord& OutSetting);

	////////////////////////////////////// Append
	//TODO move to library

	//If "A" contains a setting it will be overwritten by setting from "B"
	UFUNCTION(BlueprintCallable, Category="GameSettings|AppendSettings")
	void AppendBoolSettings(const TMap<FGameplayTag, bool>& A,const TMap<FGameplayTag, bool>& B,  TMap<FGameplayTag, bool>& Result);

	//If "A" contains a setting it will be overwritten by setting from "B"
	UFUNCTION(BlueprintCallable, Category="GameSettings|AppendSettings")
	void AppendIntSettings(const TMap<FGameplayTag, int32>& A,const TMap<FGameplayTag, int32>& B,  TMap<FGameplayTag, int32>& Result);

	//If "A" contains a setting it will be overwritten by setting from "B"
	UFUNCTION(BlueprintCallable, Category="GameSettings|AppendSettings")
	void AppendFloatSettings(const TMap<FGameplayTag, float>& A,const TMap<FGameplayTag, float>& B,  TMap<FGameplayTag, float>& Result);

	//If "A" contains a setting it will be overwritten by setting from "B"
	UFUNCTION(BlueprintCallable, Category="GameSettings|AppendSettings")
	void AppendInputSettings(const TMap<FGameplayTag, FInputChord>& A,const TMap<FGameplayTag, FInputChord>& B,  TMap<FGameplayTag, FInputChord>& Result);

};
