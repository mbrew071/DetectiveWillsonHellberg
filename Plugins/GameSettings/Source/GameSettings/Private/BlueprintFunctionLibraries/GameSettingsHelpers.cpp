// All rights reserved hehe

#include "Engine/Engine.h"
#include "BlueprintFunctionLibraries/GameSettingsHelpers.h"

bool UGameSettingsHelpers::EnumToResolution(const EScreenResolutionEnum InScreenResolutionEnum, FIntPoint& OutRes)
{
	// Get enum ptr
	UEnum* EnumPtr = StaticEnum<EScreenResolutionEnum>();
	if (!EnumPtr) { return false; }

	//Convert the InScreenResolutionEnum to string, Example value "16:10, 1280x800 (WXGA)"
	FString EnumString = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(InScreenResolutionEnum)).ToString();
	
	// Attempt to find the resolution substring in the format "WxH"
	int32 IndexOfComma = EnumString.Find(TEXT(","), ESearchCase::IgnoreCase, ESearchDir::FromStart);
	if (IndexOfComma != INDEX_NONE)
	{
		FString ResolutionString = EnumString.Mid(IndexOfComma + 2); // Skip ", "
		int32 IndexOfSpace = ResolutionString.Find(TEXT(" "), ESearchCase::IgnoreCase, ESearchDir::FromStart);
		if (IndexOfSpace != INDEX_NONE)
		{
			ResolutionString = ResolutionString.Left(IndexOfSpace);
		}

		// Now ResolutionString should be something like "1280x720"
		FString XString, YString;
		if (ResolutionString.Split(TEXT("x"), &XString, &YString))
		{
			int32 X = FCString::Atoi(*XString);
			int32 Y = FCString::Atoi(*YString);

			OutRes = FIntPoint(X, Y);
			return true;
		}
	}

	// If we reach here, something went wrong
	return false;
}

bool UGameSettingsHelpers::ResolutionToEnum(const FIntPoint InRes, EScreenResolutionEnum& OutScreenResolutionEnum)
{
	//Get the Res as string
	FString ResString = FString::FromInt(InRes.X) + "x" + FString::FromInt(InRes.Y);

	// Get enum ptr
	UEnum* EnumPtr = StaticEnum<EScreenResolutionEnum>();
	if (!EnumPtr)
	{
		return false;
	}
	
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); i++)
	{
		// Get the enum value as text (DisplayName)
		FString EnumString = EnumPtr->GetDisplayNameTextByIndex(i).ToString();
		// Compare the enum resolution to the "ResString"
		if (EnumString.Contains(ResString))
		{
			OutScreenResolutionEnum = static_cast<EScreenResolutionEnum>(i);
			return true;
		}
	}

	// If we reach here, no match was found
	OutScreenResolutionEnum = EScreenResolutionEnum::Custom;
	return false;
}

bool UGameSettingsHelpers::IntToResolutionEnum(const int32 Index, EScreenResolutionEnum& OutScreenResolutionEnum)
{
	// Get enum ptr
	UEnum* EnumPtr = StaticEnum<EScreenResolutionEnum>();
	if (!EnumPtr)
	{
		return false;
	}

	// Ensure the index is within the valid range
	if(Index >= 0 && Index < EnumPtr->GetMaxEnumValue())
	{
		// Retrieve the enum value by index
		OutScreenResolutionEnum = static_cast<EScreenResolutionEnum>(EnumPtr->GetValueByIndex(Index));
		return true;
	}

	// Index out of range
	return false;
}

bool UGameSettingsHelpers::ResolutionStringToEnum(const FString StringResolutionEnum, EScreenResolutionEnum& OutScreenResolutionEnum)
{
	// Get enum ptr
	UEnum* EnumPtr = StaticEnum<EScreenResolutionEnum>();
	if (!EnumPtr) { return false; }

	// "StringResolutionEnum" will have value of eg. "16:10, 1440x900 (WXGA+)"
	// Iterate through all enum values
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); i++)
	{
		const FString EnumDisplayName = EnumPtr->GetDisplayNameTextByIndex(i).ToString();
        
		// Compare the enum display name to the input string
		if (EnumDisplayName.Equals(StringResolutionEnum, ESearchCase::IgnoreCase))
		{
			OutScreenResolutionEnum = static_cast<EScreenResolutionEnum>(i);
			return true; // Found a match
		}
	}
	// If we reach here, no match was found
	return false;
}

bool UGameSettingsHelpers::WindowModeStringToEnum(const FString& WindowModeString, TEnumAsByte<EWindowMode::Type>& OutWindowMode)
{
	// Get enum ptr
	UEnum* EnumPtr = StaticEnum<EWindowMode::Type>();
	if (!EnumPtr) { return false; }

	// "StringResolutionEnum" will have value of eg. "16:10, 1440x900 (WXGA+)"
	// Iterate through all enum values
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); i++)
	{
		const FString EnumDisplayName = EnumPtr->GetDisplayNameTextByIndex(i).ToString();
        
		// Compare the enum display name to the input string
		if (EnumDisplayName.Equals(WindowModeString, ESearchCase::IgnoreCase))
		{
			OutWindowMode = static_cast<EWindowMode::Type>(i);
			return true; // Found a match
		}
	}
	// If we reach here, no match was found
	return false;
}

bool UGameSettingsHelpers::IntToWindowModeEnum(const int32& WindowModeInt, TEnumAsByte<EWindowMode::Type>& OutWindowMode)
{
	// Ensure the int value is within the range of EWindowMode::Type
	if(WindowModeInt >= EWindowMode::Fullscreen && WindowModeInt <= EWindowMode::WindowedFullscreen)
	{
		// Cast the int to EWindowMode::Type and assign it
		OutWindowMode = static_cast<EWindowMode::Type>(WindowModeInt);
		return true;
	}

	// The provided int doesn't map to a valid EWindowMode::Type
	return false;
}

bool UGameSettingsHelpers::GetGraphicsSettingsFromEngine(FGraphicsSettings& GraphicSettingsCombined)
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
//TODO
//	GraphicSettingsCombined.
	return true;
}

bool UGameSettingsHelpers::SplitGameplayTag(const FGameplayTag& GameplayTag, FGameplayTag& OutGameplayTag,
	 int32 PartIndex)
{
	//Do not allow negative 
	PartIndex = FMath::Max(PartIndex, 0);
	const FString SplitString = ".";
	
	FString InputString = GameplayTag.ToString();
	FString ReturnString;
	
	FString LeftS, RightS;

	if (!InputString.Split(SplitString, &LeftS, &RightS))
	{
		OutGameplayTag = FGameplayTag::RequestGameplayTag(FName(InputString));
		return OutGameplayTag.IsValid();
	}
	//GameSettings.Input.xdd
	for (int i = 0; i <= PartIndex; ++i)
	{
		if (!InputString.Split(SplitString, &LeftS, &RightS))
		{
			ReturnString += "." + InputString;
			break;
		}
		
		if (ReturnString.IsEmpty())
		{
			ReturnString += LeftS;
		}
		else
		{
			ReturnString += "." + LeftS;
		}
		InputString = *RightS;
	}
	OutGameplayTag = FGameplayTag::RequestGameplayTag(FName(ReturnString));
	return OutGameplayTag.IsValid();
}

void UGameSettingsHelpers::GetLastPartIndex(const FGameplayTag GameplayTag, int32& LastPartIndex)
{
	if (!GameplayTag.IsValid())
	{
		LastPartIndex = -1;
		return;
	}
	
	LastPartIndex = 0;
	FString StringGameplayTag = GameplayTag.ToString();

	FString RString;
	FString LString;
	
	for (int i = 0; i < StringGameplayTag.Len(); ++i)
	{
		if (StringGameplayTag.Split(".", &LString, &RString))
		{
			LastPartIndex++;
			StringGameplayTag = RString;
		}
		else
		{
			break;
		}
	}
	return;
}
