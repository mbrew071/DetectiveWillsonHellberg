// All rights reserved KAG

#include "Subsystems/GameSettingsController.h"
#include "Objects/SettingsUIController.h"

//Json serialization
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

#include "Misc/Paths.h"

#include "Kismet/GameplayStatics.h"



#include "JsonObjectConverter.h"
#include "Interfaces/InputSettingsUIController_I.h"
#include "Interfaces/GraphicsSettingsUIController_I.h"
#include "Objects/InputSettingsUIController.h"
#include "Objects/GraphicsSettingsUIController.h"
#include "Structures/GenericSettings.h"

bool UGameSettingsController::InitializePlugin(TSoftClassPtr<UGraphicsSettingsUIController> InGraphicsSettingsClass, TSoftClassPtr<USettingsUIController> InAudioSettingsClass, TSoftClassPtr<USettingsUIController> InGameSettingsClass, TSoftClassPtr<UInputSettingsUIController> InBindsSettingsClass, const TArray<FGameplayTag> InJsonSerializedSettings, FString InJsonSerializationPath, const TSet<FGameplayTag> InSaveGameSettings)
{
	bool bSuccess =true;
	//Graphics
	if (InGraphicsSettingsClass.IsNull())
	{
		bSuccess = false;
	}
	else
	{
		GraphicsSettingsClass = InGraphicsSettingsClass;
	}
	//Audio
	if (InAudioSettingsClass.IsNull())
	{
		bSuccess = false;
	}
	else
	{
		AudioSettingsClass = InAudioSettingsClass;
	}

	//Game
	if (InAudioSettingsClass.IsNull())
	{
		bSuccess = false;
	}
	else
	{
		GeneralSettingsClass = InGameSettingsClass;
	}

	//Binds
	if (InBindsSettingsClass.IsNull())
	{
		bSuccess = false;
	}
	else
	{
		BindsSettingsClass = InBindsSettingsClass;
	}

	if(InJsonSerializationPath.IsEmpty())
	{
		bSuccess = false;
		SettingsSavePath = FPaths::ProjectSavedDir();
	}
	else
	{
		SettingsSavePath = InJsonSerializationPath;
	}
	
	SaveGameSettings = InSaveGameSettings;
	JsonSerializedSettings = InJsonSerializedSettings;
	return bSuccess;
}

bool UGameSettingsController::InitializeSettings(const TMap<FGameplayTag, bool>& DefaultBoolSettings,const TMap<FGameplayTag, int32>& DefaultIntSettings, const TMap<FGameplayTag, float>& DefaultFloatSettings, const TMap<FGameplayTag, FInputChord>& DefaultInputSettings)
{
	InitializeGenericSettings(DefaultBoolSettings, DefaultIntSettings, DefaultFloatSettings);
	InitializeInputSettings(DefaultInputSettings);
	return true;
}

/*
bool UGameSettingsController::TryFindPreviousSettings(const TArray<FGameplayTag>& RequiredSettings, TMap<FGameplayTag, bool>& OutBoolSettings,TMap<FGameplayTag, int32>& OutIntSettings,TMap<FGameplayTag, float>& OutFloatSettings, TMap<FGameplayTag, FInputChord>& OutInputSettings)
{
	//Deserialize GenericSettings 
	TMap<FGameplayTag, FGenericSettings> DeserializedGenericSettings;
	if (DeserializeGenericSettings(DeserializedGenericSettings, SettingsSavePath) == false)
	{
		UE_LOG(LogTemp,Warning, TEXT("absent or corrupted genericSettings file at path %s"), *SettingsSavePath);
		return false;
	}

	//Deserialize KeybindingSettings 
	TMap<FGameplayTag, FInputChordSettings> DeserializedKeybindingSettings;
	if (DeserializeKeybindingSettings(DeserializedKeybindingSettings, SettingsSavePath) == false )
	{
		UE_LOG(LogTemp,Warning, TEXT("absent or corrupted InputSettings file at path %s"), *SettingsSavePath);
		return false;
	}
	
	//FGenericSettings* GenericSettings ;
	FGenericSettings* GenericSettings = DeserializedGenericSettings.Find(FGameplayTag::RequestGameplayTag(FName("GameSettings")));
	if(GenericSettings == nullptr)
	{
		return false;
	}
	TMap <FGameplayTag, bool> BoolSettingsMap = GenericSettings->BoolSettings;
	TMap <FGameplayTag, int32> IntSettingsMap = GenericSettings->IntSettings;
	TMap <FGameplayTag, float> FloatSettingsMap = GenericSettings->FloatSettings;

	FInputChordSettings* InputChordSettings = DeserializedKeybindingSettings.Find(FGameplayTag::RequestGameplayTag(FName("GameSettings.Input")));
	if (InputChordSettings ==nullptr)
	{
		return false;
	}
	TMap<FGameplayTag, FInputChord> InputSettingsMap = InputChordSettings->Inputs;

	
	//are all required Settings in files?
	for (auto& Setting : RequiredSettings)
	{
		if (bool* bBool = BoolSettingsMap.Find(Setting))
		{
			OutBoolSettings.Add(Setting, *bBool);
			continue;
		}
		if(int32* Int = IntSettingsMap.Find(Setting))
		{
			OutIntSettings.Add(Setting, *Int);
			continue;
		}
		if(float* Float = FloatSettingsMap.Find(Setting))
		{
			OutFloatSettings.Add(Setting, *Float);
			continue;
		}
		if(FInputChord* InputChord = InputSettingsMap.Find(Setting))
		{
			OutInputSettings.Add(Setting, *InputChord);
			continue;
		}
		UE_LOG(LogTemp, Warning, TEXT("Could not find setting %s"), *Setting.ToString());
	}
	return true;
}
*/

bool UGameSettingsController::InitializeGenericSettings(const TMap<FGameplayTag, bool>& DefaultBoolSettings,const TMap<FGameplayTag, int32>& DefaultIntSettings, const TMap<FGameplayTag, float>& DefaultFloatSettings)
{
	TMap<FGameplayTag, bool> JsonBoolSettings;
	TMap<FGameplayTag, int32> JsonIntSettings;
	TMap<FGameplayTag, float> JsonFloatSettings;
	TryFindPreviousGenericSettings(JsonSerializedSettings, JsonBoolSettings, JsonIntSettings, JsonFloatSettings);

	TMap<FGameplayTag, bool> CombinedBoolSettings;
	TMap<FGameplayTag, int32> CombinedIntSettings;
	TMap<FGameplayTag, float> CombinedFloatSettings;
	AppendBoolSettings(DefaultBoolSettings,JsonBoolSettings, CombinedBoolSettings);
	AppendIntSettings(DefaultIntSettings, JsonIntSettings, CombinedIntSettings);
	AppendFloatSettings(DefaultFloatSettings, JsonFloatSettings, CombinedFloatSettings);
	
	SetBoolSettings(CombinedBoolSettings);
	SetIntSettings(CombinedIntSettings);
	SetFloatSettings(CombinedFloatSettings);
	
	//Apply Graphics settings
	UObject* GraphicsSettingsController;
	GetGraphicSettingsController(this, GraphicsSettingsController);
	if (GraphicsSettingsController->GetClass()->ImplementsInterface(UGraphicsSettingsUIController_I::StaticClass()))
	{
		IGraphicsSettingsUIController_I::Execute_ApplyGraphicsSettings(GraphicsSettingsController, CombinedBoolSettings, CombinedIntSettings, CombinedFloatSettings);	
	}
	return true;
}

bool UGameSettingsController::InitializeInputSettings(const TMap<FGameplayTag, FInputChord>& DefaultInputSettings)
{
	TMap<FGameplayTag, FInputChord> JsonInputSettings;
	TryFindPreviousInputSettings(JsonSerializedSettings, JsonInputSettings);
	
	TMap<FGameplayTag, FInputChord> CombinedInputSettings;
	AppendInputSettings(DefaultInputSettings, JsonInputSettings, CombinedInputSettings);
	
	SetInputSettings(CombinedInputSettings);
	
	//Apply Input settings
	UObject* InputSettingsUIController;
	GetBindsController(this, InputSettingsUIController, UGameplayStatics::GetPlayerController(this, 0));
	if (InputSettingsUIController->GetClass()->ImplementsInterface(UInputSettingsUIController_I::StaticClass()))
	{
		IInputSettingsUIController_I::Execute_ApplyInputSettings(InputSettingsUIController,CombinedInputSettings);
	}
	return true;
}

bool UGameSettingsController::TryFindPreviousGenericSettings(const TArray<FGameplayTag>& RequiredSettings, TMap<FGameplayTag, bool>& OutBoolSettings, TMap<FGameplayTag, int32>& OutIntSettings, TMap<FGameplayTag, float>& OutFloatSettings)
{
	OutBoolSettings.Empty();
	OutIntSettings.Empty();
	OutFloatSettings.Empty();
	
	//Deserialize GenericSettings 
	TMap<FGameplayTag, FGenericSettings> DeserializedGenericSettings;
	if (DeserializeGenericSettings(DeserializedGenericSettings, SettingsSavePath) == false)
	{
		UE_LOG(LogTemp,Warning, TEXT("absent or corrupted genericSettings file at path %s"), *SettingsSavePath);
		return false;
	}
	
	FGenericSettings* GenericSettings = DeserializedGenericSettings.Find(FGameplayTag::RequestGameplayTag(FName("GameSettings")));
	if(GenericSettings == nullptr)
	{
		return false;
	}

	//Read only Required settings from the json file
	//We want to ignore others in case user wants to manually add some bullshit to json
	for (auto& Tag : RequiredSettings)
	{
		if (bool* bBool = GenericSettings->BoolSettings.Find(Tag))
		{
			OutBoolSettings.Add(Tag, *bBool);
			continue;
		}
		if(int32* Int = GenericSettings->IntSettings.Find(Tag))
		{
			OutIntSettings.Add(Tag, *Int);
			continue;
		}
		if(float* Float = GenericSettings->FloatSettings.Find(Tag))
		{
			OutFloatSettings.Add(Tag, *Float);
			continue;
		}
		UE_LOG(LogTemp, Warning, TEXT("Could not find setting %s"), *Tag.ToString());
	}
	return true;
	
}

bool UGameSettingsController::TryFindPreviousInputSettings(const TArray<FGameplayTag>& RequiredSettings,TMap<FGameplayTag, FInputChord>& OutInputSettings)
{
	OutInputSettings.Empty();
	//Deserialize KeybindingSettings 
	TMap<FGameplayTag, FInputChordSettings> DeserializedKeybindingSettings;
	if (DeserializeKeybindingSettings(DeserializedKeybindingSettings, SettingsSavePath) == false )
	{
		UE_LOG(LogTemp,Warning, TEXT("absent or corrupted InputSettings file at path %s"), *SettingsSavePath);
		return false;
	}

	FInputChordSettings* InputChordSettings = DeserializedKeybindingSettings.Find(FGameplayTag::RequestGameplayTag(FName("GameSettings.Input")));
	if (InputChordSettings ==nullptr)
	{
		return false;
	}

	//Read only Required settings from the json file
	//We want to ignore others in case user wants to manually add some bullshit to json
	for (auto& Tag : RequiredSettings)
	{
		if (FInputChord* Input = InputChordSettings->Inputs.Find(Tag))
		{
			OutInputSettings.Add(Tag, *Input);
			continue;
		}
		UE_LOG(LogTemp, Warning, TEXT("Could not find setting %s"), *Tag.ToString());
	}
	return true;
}

//////////////////////// GET Controllers:

bool UGameSettingsController::GetGraphicSettingsController(const UObject* WorldContextObject, UObject*& OutGraphicsSettingsController)
{
	if (GraphicsSettingsClass.IsNull())
	{
		return false;
	}
	if (WorldContextObject == nullptr)
	{
		return false;
	}
	
	USettingsUIController* Object = NewObject<USettingsUIController>(this, GraphicsSettingsClass.LoadSynchronous());

	Object->GameSettingsController = this;
	Object->Owner = WorldContextObject;
	OutGraphicsSettingsController = Object;
	Object->CustomConstruction();
	return true;
}

bool UGameSettingsController::GetAudioSettingsController(const UObject* WorldContextObject, UObject*& OutAudioSettingsController)
{
	if (AudioSettingsClass.IsNull())
	{
		return false;
	}
	if (WorldContextObject == nullptr)
	{
		return false;
	}
	
	USettingsUIController* Object = NewObject<USettingsUIController>(this, AudioSettingsClass.LoadSynchronous());
	
	Object->GameSettingsController = this;
	Object->Owner = WorldContextObject;
	OutAudioSettingsController = Object;
	Object->CustomConstruction();
	return true;
}

bool UGameSettingsController::GetGeneralSettingsController(const UObject* WorldContextObject, UObject*& OutGeneralSettingsController)
{
	if (GeneralSettingsClass.IsNull())
	{
		return false;
	}
	if (WorldContextObject == nullptr)
	{
		return false;
	}
	
	USettingsUIController* Object = NewObject<USettingsUIController>(this, GeneralSettingsClass.LoadSynchronous());
	
	Object->GameSettingsController = this;
	Object->Owner = WorldContextObject;
	OutGeneralSettingsController = Object;
	Object->CustomConstruction();
	return true;
}

bool UGameSettingsController::GetBindsController(const UObject* WorldContextObject, UObject*& OutBindsSettingsController, AController* PlayerController)
{
	if (BindsSettingsClass.IsNull())
	{
		return false;
	}
	if (WorldContextObject == nullptr)
	{
		return false;
	}
	
	UInputSettingsUIController* Object = NewObject<UInputSettingsUIController>(this, BindsSettingsClass.LoadSynchronous());

	Object->GameSettingsController = this;
	Object->Owner = WorldContextObject;
	Object->PlayerController = PlayerController;
	OutBindsSettingsController = Object;
	Object->CustomConstruction();
	return true;
}

bool UGameSettingsController::SerializeInputTemp()
{
	TMap<FGameplayTag, FInputChord> InputSettingsToSerialize;

	for (FGameplayTag& Tag : JsonSerializedSettings)
	{
		FInputChord InputChord;
		if(FindKeybindingSettingByTag(Tag, GetInputSettings(), InputChord))
		{
			InputSettingsToSerialize.Add(Tag, InputChord);
		}
	}
	
	FInputChordSettings InputChordSettings;
	InputChordSettings.Inputs = InputSettingsToSerialize;

	TMap<FGameplayTag, FInputChordSettings> InputMapToSerialize;
	InputMapToSerialize.Add(FGameplayTag::RequestGameplayTag(FName("GameSettings.Input"),false), InputChordSettings);
	if (!SerializeKeybindingSettings(InputMapToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT("InputSettings Serialization failed"));
	}
	return true;
}

bool UGameSettingsController::SerializeGenericTemp()
{
	//TODO change that in future
	FGenericSettings GenericSettingsToSerialize;
	
	TMap<FGameplayTag, bool> BoolSettingsToSerialize;
	TMap<FGameplayTag, int32> IntSettingsToSerialize;
	TMap<FGameplayTag, float> FloatSettingsToSerialize;

	for (FGameplayTag& Tag : JsonSerializedSettings)
	{
		bool bBool;
		if (FindGenericBoolSettingByTag(Tag, GetBoolSettings(), bBool))
		{
			BoolSettingsToSerialize.Add(Tag, bBool);
			continue;
		}
		int32 Int;
		if( FindGenericIntSettingByTag(Tag, GetIntSettings(), Int))
		{
			IntSettingsToSerialize.Add(Tag, Int);
			continue;
		}
		float Float;
		if(FindGenericFloatSettingByTag(Tag, GetFloatSettings(),Float))
		{
			FloatSettingsToSerialize.Add(Tag, Float);
			continue;
		}
		UE_LOG(LogTemp,Warning, TEXT("Setting %s not found, therefore will not be serialized into json"), *Tag.ToString());
	}

	GenericSettingsToSerialize.BoolSettings = BoolSettingsToSerialize;
	GenericSettingsToSerialize.IntSettings = IntSettingsToSerialize;
	GenericSettingsToSerialize.FloatSettings = FloatSettingsToSerialize;

	TMap<FGameplayTag, FGenericSettings> GenericMapToSerialize;
	GenericMapToSerialize.Add(FGameplayTag::RequestGameplayTag(FName("GameSettings"),false),GenericSettingsToSerialize);
	if (!SerializeGenericSettings(GenericMapToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT("GenericSettings Serialization failed"));
	}
	return true;
}

////////////////////////// JSON Serialization:

bool UGameSettingsController::SerializeKeybindingSettings(const TMap<FGameplayTag, FInputChordSettings>& InKeybindingSettings, const FString& InFilePath, const FString InFileName)
{
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);

	for (const auto& Elem : InKeybindingSettings) // Make sure to iterate over InKeybindingSettings
		{
		FString KeyString = Elem.Key.ToString();
		const FInputChordSettings& Settings = Elem.Value;

		FString OutputString;
		if (FJsonObjectConverter::UStructToJsonObjectString(FInputChordSettings::StaticStruct(), &Settings, OutputString, 0, 0))
		{
			// Parse the string to a Json Object
			TSharedPtr<FJsonObject> SettingsObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutputString);
			if (FJsonSerializer::Deserialize(Reader, SettingsObject) && SettingsObject.IsValid())
			{
				RootObject->SetObjectField(KeyString, SettingsObject);
			}
		}
		}

	FString FinalString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&FinalString);
	if (FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
	{
		return FFileHelper::SaveStringToFile(FinalString, *(InFilePath + InFileName));
	}

	return false;
}

bool UGameSettingsController::DeserializeKeybindingSettings(TMap<FGameplayTag, FInputChordSettings>& OutKeybindings, const FString& InFilePath, const FString InFileName)
{
	FString FullPath = FPaths::Combine(InFilePath, InFileName);
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
	{
		return false;
	}

	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		return false;
	}

	for (const auto& Elem : RootObject->Values)
	{
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*Elem.Key), false);
		if (!Tag.IsValid())
		{
			continue; // Skip if we can't properly convert the string back to a FGameplayTag
		}

		const TSharedPtr<FJsonObject>* SettingsObjectPtr;
		if (!Elem.Value->TryGetObject(SettingsObjectPtr))
		{
			continue;
		}

		FInputChordSettings Settings;
		FString SettingsJsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SettingsJsonString);
		if (FJsonSerializer::Serialize(SettingsObjectPtr->ToSharedRef(), Writer))
		{
			if (FJsonObjectConverter::JsonObjectStringToUStruct(SettingsJsonString, &Settings, 0, 0))
			{
				OutKeybindings.Add(Tag, Settings);
			}
		}
	}

	return true;
}

bool UGameSettingsController::SerializeGenericSettings(const TMap<FGameplayTag, FGenericSettings>& InGameSettings, const FString& InFilePath, const FString InFileName)
{
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);

	for (const auto& Elem : InGameSettings)
	{
		FString KeyString = Elem.Key.ToString();
		const FGenericSettings& Settings = Elem.Value;

		FString OutputString;
		if (FJsonObjectConverter::UStructToJsonObjectString(FGenericSettings::StaticStruct(), &Settings, OutputString, 0, 0))
		{
			// Parse the string to a Json Object
			TSharedPtr<FJsonObject> SettingsObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutputString);
			if (FJsonSerializer::Deserialize(Reader, SettingsObject) && SettingsObject.IsValid())
			{
				RootObject->SetObjectField(KeyString, SettingsObject);
			}
		}
	}

	FString FinalString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&FinalString);
	if (FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
	{
		return FFileHelper::SaveStringToFile(FinalString, *(InFilePath + InFileName));
	}

	return false;
}

bool UGameSettingsController::DeserializeGenericSettings(TMap<FGameplayTag, FGenericSettings>& OutGenericSettings, const FString& InFilePath, const FString InFileName)
{
	/*FString ReadString;
	if (FFileHelper::LoadFileToString(ReadString, *(InFilePath +InFileName)))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct<FGenericSettings>(ReadString, &OutGameSettings, 0, 0);
		return true;
	}
	return false;*/
	
	FString FullPath = FPaths::Combine(InFilePath, InFileName);
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
	{
		return false;
	}

	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		return false;
	}

	for (const auto& Elem : RootObject->Values)
	{
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*Elem.Key), false);
		if (!Tag.IsValid())
		{
			continue; // Skip if we can't properly convert the string back to a FGameplayTag
		}

		FGenericSettings Settings;
		const TSharedPtr<FJsonValue>& JsonValuePtr = Elem.Value;

		if (JsonValuePtr.IsValid() && JsonValuePtr->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> SettingsJsonObject = JsonValuePtr->AsObject();
			if (SettingsJsonObject.IsValid())
			{
				FJsonObjectConverter::JsonObjectToUStruct<FGenericSettings>(SettingsJsonObject.ToSharedRef(), &Settings, 0, 0);
				OutGenericSettings.Add(Tag, Settings);
			}
		}
	}
	return true;
}

/////////////////////// Get/Set All Confirmed Settings:

bool UGameSettingsController::GetConfirmedSettings(TMap<FGameplayTag, bool>& OutBoolSettings,TMap<FGameplayTag, int32>& OutIntSettings, TMap<FGameplayTag, float>& OutFloatSettings,TMap<FGameplayTag,FInputChord>& OutKeybindingSettings)
{
	OutBoolSettings = GetBoolSettings();
	OutIntSettings = GetIntSettings();
	OutFloatSettings = GetFloatSettings();
	OutKeybindingSettings = GetInputSettings();
	return true;
}

TMap<FGameplayTag, bool> UGameSettingsController::GetBoolSettings()
{
	TMap<FGameplayTag, bool> Temp = BoolSettings;
	Temp.Append(BoolSettingsSerializable);
	return Temp;
}

TMap<FGameplayTag, int32> UGameSettingsController::GetIntSettings()
{
	TMap<FGameplayTag, int32> Temp = IntSettings;
	Temp.Append(IntSettingsSerializable);
	return Temp;
}

TMap<FGameplayTag, float> UGameSettingsController::GetFloatSettings()
{
	TMap<FGameplayTag, float> Temp = FloatSettings;
	Temp.Append(FloatSettingsSerializable);
	return Temp;
}

/*
bool UGameSettingsController::GetConfirmedSettingsMerge(TMap<FGameplayTag, FGenericSettings>& OutGenericSettings, TMap<FGameplayTag, FInputChordSettings>& OutKeybindingSettings)
{
		//Deserialize GenericSettings 
	TMap<FGameplayTag, FGenericSettings> DeserializedGenericSettings;
	DeserializeGenericSettings(DeserializedGenericSettings, SettingsSavePath);

	//Get settings that can be loaded from json
	TMap<FGameplayTag, FGenericSettings> DeserializedSettingsToConsider;
	FindGenericSettingsByTag(JsonSerializedSettings, DeserializedGenericSettings, DeserializedSettingsToConsider);
	
	//Replace variable stored settings with json stored settings  
	TMap<FGameplayTag, FGenericSettings> TempGenericSettings = GenericSettings;
	for (auto& Element : DeserializedSettingsToConsider)
	{
		for (auto& Element2 : Element.Value.BoolSettings)
		{
			TempGenericSettings.FindOrAdd(Element.Key).BoolSettings.Add(Element2.Key, Element2.Value);
		}
		for (auto& Element2 : Element.Value.IntSettings)
		{
			TempGenericSettings.FindOrAdd(Element.Key).IntSettings.Add(Element2.Key, Element2.Value);
		}
		for (auto& Element2 : Element.Value.FloatSettings)
		{
			TempGenericSettings.FindOrAdd(Element.Key).FloatSettings.Add(Element2.Key, Element2.Value);
		}
	}
	OutGenericSettings = MoveTemp(TempGenericSettings);

	//Deserialize KeybindingSettings 
	TMap<FGameplayTag, FInputChordSettings> DeserializedKeybindingSettings;
	DeserializeKeybindingSettings(DeserializedKeybindingSettings, SettingsSavePath);

	//Get settings that can be loaded from json
	TMap<FGameplayTag, FInputChordSettings> DeserializedKeybindingSettingsToConsider;
	FindKeybindingSettingsByTag(JsonSerializedSettings, DeserializedKeybindingSettings, DeserializedKeybindingSettingsToConsider);
	
	//Replace variable stored settings with json stored settings
	TMap<FGameplayTag, FInputChordSettings> TempKeybindingSettings = KeybindingSettings;
	for (auto& Element : DeserializedKeybindingSettingsToConsider)
	{
		for (auto& Element2 : Element.Value.Inputs)
		{
			TempKeybindingSettings.FindOrAdd(Element.Key).Inputs.Add(Element2.Key, Element2.Value);
		}
	}
	OutKeybindingSettings = MoveTemp(TempKeybindingSettings);
	return true;
}
*/

TMap<FGameplayTag, FInputChord> UGameSettingsController::GetInputSettings()
{
	return KeybindingSettings;
}

bool UGameSettingsController::SetConfirmedSettings(const TMap<FGameplayTag, bool> InBoolSettings, const TMap<FGameplayTag, int32> InIntSettings,const TMap<FGameplayTag, float> InfloatSettings, const TMap<FGameplayTag, FInputChord> InInputSettings)
{
	//Save to variables
	SetBoolSettings(InBoolSettings);
	SetIntSettings(InIntSettings);
	SetFloatSettings(InfloatSettings);
	SetInputSettings(InInputSettings);
	
	return true;
	
	/*TMap<FGameplayTag, bool> BoolSettingsToSerialize;
	TMap<FGameplayTag, int32> IntSettingsToSerialize;
	TMap<FGameplayTag, float> FloatSettingsToSerialize;
	TMap<FGameplayTag, FInputChord> InputSettingsToSerialize;

	for (FGameplayTag& Tag : JsonSerializedSettings)
	{
		bool bBool;
		if (FindGenericBoolSettingByTag(Tag, GetBoolSettings(), bBool))
		{
			BoolSettingsToSerialize.Add(Tag, bBool);
			continue;
		}
		int32 Int;
		if( FindGenericIntSettingByTag(Tag, GetIntSettings(), Int))
		{
			IntSettingsToSerialize.Add(Tag, Int);
			continue;
		}
		float Float;
		if(FindGenericFloatSettingByTag(Tag, GetFloatSettings(),Float))
		{
			FloatSettingsToSerialize.Add(Tag, Float);
			continue;
		}
		FInputChord InputChord;
		if(FindKeybindingSettingByTag(Tag, GetInputSettings(), InputChord))
		{
			InputSettingsToSerialize.Add(Tag, InputChord);
		}
		UE_LOG(LogTemp,Warning, TEXT("Setting %s not found, therefore will not be serialized into json"), *Tag.ToString());
	}


	
	/*FGenericSettings GenericSettingsToSerialize;
	GenericSettingsToSerialize.BoolSettings = BoolSettingsToSerialize;
	GenericSettingsToSerialize.IntSettings = IntSettingsToSerialize;
	GenericSettingsToSerialize.FloatSettings = FloatSettingsToSerialize;

	TMap<FGameplayTag, FGenericSettings> GenericMapToSerialize;
	GenericMapToSerialize.Add(FGameplayTag::RequestGameplayTag(FName("GameSettings"),false),GenericSettingsToSerialize);
	if (!SerializeGenericSettings(GenericMapToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT("GenericSettings Serialization failed"));
	}

	//TODO change that in future?
	FInputChordSettings InputChordSettings;
	InputChordSettings.Inputs = InputSettingsToSerialize;

	TMap<FGameplayTag, FInputChordSettings> InputMapToSerialize;
	InputMapToSerialize.Add(FGameplayTag::RequestGameplayTag(FName("GameSettings.Input"),false), InputChordSettings);
	if (!SerializeKeybindingSettings(InputMapToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT("InputSettings Serialization failed"));
	}*/
	return true;
	
	/*//Find settings to serialize into json
	TMap<FGameplayTag, FGenericSettings> GenericSettingsToSerialize;
	//Serialize them
	if (!SerializeGenericSettings(GenericSettingsToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT(" GenericSettings Serialization Serialization failed"));
	}

	//Find InputSettings to serialize
	TMap<FGameplayTag, FInputChordSettings> KeybindingSettingsToSerialize;
	FindKeybindingSettingsByTag(JsonSerializedSettings, KeybindingSettings, KeybindingSettingsToSerialize);
	//Serialize them
	if (!SerializeKeybindingSettings(KeybindingSettingsToSerialize, SettingsSavePath))
	{
		UE_LOG(LogTemp, Error, TEXT(" KeybindingSettings Serialization Serialization failed"));
	}

	return true;*/
}

bool UGameSettingsController::SetGenericSettings(const TMap<FGameplayTag, bool> InBoolSettings, const TMap<FGameplayTag, int32> InIntSettings, const TMap<FGameplayTag, float> InfloatSettings)
{
	SetBoolSettings(InBoolSettings);
	SetIntSettings(InIntSettings);
	SetFloatSettings(InfloatSettings);
	return true;
}

void UGameSettingsController::SetBoolSettings(const TMap<FGameplayTag, bool> InBoolSettings)
{
	for (auto& Element : InBoolSettings)
	{
		if (SaveGameSettings.Contains(Element.Key))
		{
			//Add to serializable Map
			BoolSettingsSerializable.Add(Element.Key, Element.Value);
			continue;
		}
		//Add to not serializable map
		BoolSettings.Add(Element.Key, Element.Value);
	}
	//TODO change that in future*/
	SerializeGenericTemp();
}

void UGameSettingsController::SetIntSettings(const TMap<FGameplayTag, int32> InIntSettings)
{
	for (auto& Element : InIntSettings)
	{
		if (SaveGameSettings.Contains(Element.Key))
		{
			//Add to serializable Map
			IntSettingsSerializable.Add(Element.Key, Element.Value);
			continue;
		}
		//Add to not serializable map
		IntSettings.Add(Element.Key, Element.Value);
	}
	//TODO change that in future*/
	SerializeGenericTemp();
}

void UGameSettingsController::SetFloatSettings(const TMap<FGameplayTag, float> InFloatSettings)
{
	for (auto& Element : InFloatSettings)
	{
		if (SaveGameSettings.Contains(Element.Key))
		{
			//Add to serializable Map
			FloatSettingsSerializable.Add(Element.Key, Element.Value);
			continue;
		}
		//Add to not serializable map
		FloatSettings.Add(Element.Key, Element.Value);
	}
	//TODO change that in future*/
	SerializeGenericTemp();
}

void UGameSettingsController::SetInputSettings(const TMap<FGameplayTag, FInputChord> InInputSettings)
{
	KeybindingSettings = InInputSettings;
	//TODO change that in future*/
	SerializeInputTemp();
}

////////////////// Get confirmed settings By TAG 

/*bool UGameSettingsController::GetConfirmedGenericSettingByTag(const FGameplayTag InSearchedTag,
	TMap<FGameplayTag, FGenericSettings>& OutResult)
{
	TMap<FGameplayTag,FGenericSettings> ConfirmedGenericSettings;
	TMap<FGameplayTag,FInputChordSettings> ConfirmedKeybindingSettings;
	GetConfirmedSettings(ConfirmedGenericSettings, ConfirmedKeybindingSettings);
	
	return FindGenericSettingByTag(InSearchedTag,ConfirmedGenericSettings, OutResult);;
}

bool UGameSettingsController::GetConfirmedKeybindingSettingByTag(const FGameplayTag InSearchedTag,
	TMap<FGameplayTag, FInputChordSettings>& OutResult)
{
	TMap<FGameplayTag,FGenericSettings> ConfirmedGenericSettings;
	TMap<FGameplayTag,FInputChordSettings> ConfirmedKeybindingSettings;
	GetConfirmedSettings(ConfirmedGenericSettings, ConfirmedKeybindingSettings);
	
	return FindKeybindingSettingByTag(InSearchedTag, ConfirmedKeybindingSettings, OutResult);
}*/

bool UGameSettingsController::GetConfirmedGenericBoolSetting(const FGameplayTag Tag, bool& Setting)
{
	return FindGenericBoolSettingByTag(Tag, GetBoolSettings(), Setting);
}

bool UGameSettingsController::GetConfirmedGenericIntSetting(const FGameplayTag Tag, int32& Setting)
{
	return FindGenericIntSettingByTag(Tag, GetIntSettings(), Setting);
}

bool UGameSettingsController::GetConfirmedGenericFloatSetting(const FGameplayTag Tag, float& Setting)
{
	return FindGenericFloatSettingByTag(Tag, GetFloatSettings(), Setting);
}

bool UGameSettingsController::GetConfirmedKeybingingSetting(const FGameplayTag Tag, FInputChord& Setting)
{
	return FindKeybindingSettingByTag(Tag, GetInputSettings(), Setting);
}

/*void UGameSettingsController::AddGenericSettingsUnique(const TMap<FGameplayTag, FGenericSettings> A, const TMap<FGameplayTag, FGenericSettings> B, TMap<FGameplayTag, FGenericSettings>& Result)
{
	Result.Empty();
	Result.Append(A);
	
	// Add "B" settings
	for (auto& Element : B)
	{
		//Add key if not found 
		Result.FindOrAdd(Element.Key,Element.Value);

		//Bool Settings
		for (auto& Element1 : Element.Value.BoolSettings)
		{
			bool bBool;
			if (!FindGenericBoolSettingByTag(Element1.Key, Result, bBool))
			{	//If not found then add:
				FGenericSettings* Temp = Result.Find(Element.Key);
				Temp->BoolSettings.FindOrAdd(Element1.Key, Element1.Value);
			}
		}
		
		//IntSettings
		for (auto& Element1 : Element.Value.IntSettings)
		{
			int32 Int;
			if (!FindGenericIntSettingByTag(Element1.Key, Result, Int))
			{	//If not found then add:
				FGenericSettings* Temp = Result.Find(Element.Key);
				Temp->IntSettings.FindOrAdd(Element1.Key, Element1.Value);
			}
		}

		//Float Settings
		for (auto& Element1 : Element.Value.FloatSettings)
		{
			float Float;
			if (!FindGenericFloatSettingByTag(Element1.Key, Result, Float))
			{	//If not found then add:
				FGenericSettings* Temp = Result.Find(Element.Key);
				Temp->FloatSettings.FindOrAdd(Element1.Key, Element1.Value);
			}
		}
	}
}

void UGameSettingsController::AddInputSettingsUnique(const TMap<FGameplayTag, FInputChord>& A, const TMap<FGameplayTag, FInputChord>& B, TMap<FGameplayTag, FInputChord>& Result)
{
	Result.Empty();
	Result.Append(B);
	Result.Append(A);
}

////*////////////// Find Setting/Settings

/*bool UGameSettingsController::FindGenericSettingsByTag(const TArray<FGameplayTag>& InSearchedTags,
const TMap<FGameplayTag, FGenericSettings>& InSource, TMap<FGameplayTag, FGenericSettings>& OutResult)
{
	for (auto& SettingTag : InSearchedTags) 	// SettingTag is eg "GameSettings.Graphics.VSync"
		{
			FindGenericSettingByTag(SettingTag, InSource, OutResult);
		}
	return true;
}*/

bool UGameSettingsController::FindBoolSettingsByTag(const TArray<FGameplayTag>& InSearchedTags,
	const TMap<FGameplayTag, bool>& InSource, TMap<FGameplayTag, bool>& OutResult)
{
	OutResult.Empty();
	for (auto& SettingTag : InSearchedTags) 	// SettingTag is eg "GameSettings.Graphics.VSync"
	{
		bool bBoolSetting;
		if(FindGenericBoolSettingByTag(SettingTag, InSource, bBoolSetting))
		{
			OutResult.Add(SettingTag, bBoolSetting);
		}
	}
	return true;
}

bool UGameSettingsController::FindIntSettingsByTag(const TArray<FGameplayTag>& InSearchedTags,
	const TMap<FGameplayTag, int32>& InSource, TMap<FGameplayTag, int32>& OutResult)
{
	OutResult.Empty();
	for (auto& SettingTag : InSearchedTags) 	// SettingTag is eg "GameSettings.Graphics.VSync"
	{
		int32 IntSetting;
		if(FindGenericIntSettingByTag(SettingTag, InSource, IntSetting))
		{
			OutResult.Add(SettingTag, IntSetting);
		}
	}
	return true;
}

bool UGameSettingsController::FindFloatSettingsByTag(const TArray<FGameplayTag>& InSearchedTags,
	const TMap<FGameplayTag, float>& InSource, TMap<FGameplayTag, float>& OutResult)
{
	OutResult.Empty();
	for (auto& SettingTag : InSearchedTags) 	// SettingTag is eg "GameSettings.Graphics.VSync"
	{
		float FloatSetting;
		if(FindGenericFloatSettingByTag(SettingTag, InSource, FloatSetting))
		{
			OutResult.Add(SettingTag, FloatSetting);
		}
	}
	return true;
}

bool UGameSettingsController::FindKeybindingSettingsByTag(const TArray<FGameplayTag>& InSearchedTags, const TMap<FGameplayTag, FInputChord>& InSource, TMap<FGameplayTag, FInputChord>& OutResult)
{
	OutResult.Empty();
	
	for (auto& Tag : InSearchedTags) 	// SettingTag is eg "GameSettings.Graphics.VSync"
	{
		FInputChord InputChordSetting;
		if (FindKeybindingSettingByTag(Tag, InSource, InputChordSetting))
		{
			OutResult.Add(Tag, InputChordSetting);
		}
	}
	return true;
}

/*
bool UGameSettingsController::FindGenericSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, FGenericSettings>& InSource, TMap<FGameplayTag, FGenericSettings>& OutResult)
{
	for (auto& Element : InSource)
	{
		const FGameplayTag& SettingCategory = Element.Key;	//SettingCategory is eg "GameSettings.Graphics"
		const FGenericSettings& TempGenericSettings = Element.Value; //Struct with settings of this category
			
		if (auto* BoolSetting = TempGenericSettings.BoolSettings.Find(InSearchedTag))
		{	//bool setting found:
			OutResult.FindOrAdd(SettingCategory).BoolSettings.Add(InSearchedTag, *BoolSetting);
			return true;
		}
		if (auto* IntSetting = TempGenericSettings.IntSettings.Find(InSearchedTag))
		{	//int setting found:
			OutResult.FindOrAdd(SettingCategory).IntSettings.Add(InSearchedTag, *IntSetting);
			return true;
		}
		if (auto* FloatSetting = TempGenericSettings.FloatSettings.Find(InSearchedTag))
		{	//float setting found:
			OutResult.FindOrAdd(SettingCategory).FloatSettings.Add(InSearchedTag, *FloatSetting);
			return true;
		}
		//Setting wasn't found so continue loop to find searched "SettingTag"
	}
	return false;
}
*/

/*bool UGameSettingsController::FindKeybindingSettingByTag(const FGameplayTag& InSearchedTag, const FInputChordSettings InSource, TMap<FGameplayTag, FInputChordSettings>& OutResult)
{
	if(auto* FloatSetting = InSource.Inputs.Find(InSearchedTag))
	{
		OutSetting = *FloatSetting;
		return true;
	}
	return false;
	
	for (auto& Element : InSource)
	{
		const FGameplayTag& SettingCategory = Element.Key;	//SettingCategory is eg "GameSettings.Graphics"
		const FInputChordSettings& TempGenericSettings = Element.Value; //Struct with settings of this category
				
		if (auto* InputChord = TempGenericSettings.Inputs.Find(InSearchedTag))
		{	//bool setting found:
			OutResult.FindOrAdd(SettingCategory).Inputs.Add(InSearchedTag, *InputChord);
			return true;
		}
		//Setting wasn't found so continue loop to find searched "SettingTag"
	}
	return false;
}*/

bool UGameSettingsController::FindGenericBoolSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, bool> InSource, bool& OutSetting)
{
	if(auto* BoolSetting = InSource.Find(InSearchedTag))
	{
		OutSetting = *BoolSetting;
		return true;
	}
	return false;
}

bool UGameSettingsController::FindGenericIntSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, int32> InSource, int32& OutSetting)
{
	if(auto* IntSetting = InSource.Find(InSearchedTag))
	{
		OutSetting = *IntSetting;
		return true;
	}
	return false;
}

bool UGameSettingsController::FindGenericFloatSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, float>& InSource, float& OutSetting)
{
	if(auto* FloatSetting = InSource.Find(InSearchedTag))
	{
		OutSetting = *FloatSetting;
		return true;
	}
	return false;
}
                              
bool UGameSettingsController::FindKeybindingSettingByTag(const FGameplayTag& InSearchedTag, const TMap<FGameplayTag, FInputChord>& InSource, FInputChord& OutSetting)
{
	if(const FInputChord* InputChord = InSource.Find(InSearchedTag))
	{
		OutSetting = *InputChord;
		return true;
	}
	return false;
}

////////////////// Append settings 

void UGameSettingsController::AppendBoolSettings(const TMap<FGameplayTag, bool>& A, const TMap<FGameplayTag, bool>& B, TMap<FGameplayTag, bool>& Result)
{
	Result.Empty();
	Result.Append(A);
	Result.Append(B);
}

void UGameSettingsController::AppendIntSettings(const TMap<FGameplayTag, int32>& A, const TMap<FGameplayTag, int32>& B, TMap<FGameplayTag, int32>& Result)
{
	Result.Empty();
	Result.Append(A);
	Result.Append(B);
}

void UGameSettingsController::AppendFloatSettings(const TMap<FGameplayTag, float>& A,
	const TMap<FGameplayTag, float>& B, TMap<FGameplayTag, float>& Result)
{
	Result.Empty();
	Result.Append(A);
	Result.Append(B);
}

void UGameSettingsController::AppendInputSettings(const TMap<FGameplayTag, FInputChord>& A,
	const TMap<FGameplayTag, FInputChord>& B, TMap<FGameplayTag, FInputChord>& Result)
{
	Result.Empty();
	Result.Append(A);
	Result.Append(B);
}

