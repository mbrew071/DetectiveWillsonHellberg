// All rights reserved hehe


#include "Subsystems/ActorNarrationController.h"

bool UActorNarrationController::InitializePlugin(TEnumAsByte<ENarrationLineType> InNarrationLineType, TSoftObjectPtr<UDataTable> InTextStylePresets, TSoftObjectPtr<UDataTable> InTextAnimPresets, TSoftObjectPtr<UDataTable> InMasterNarrationDT, TSoftClassPtr<UUserWidget> InWidgetClass, TSoftObjectPtr<USoundAttenuation> InSoundAttenuation)
{
	bool bSuccess = true;
	NarrationLineType = InNarrationLineType;
	
	if (InTextStylePresets.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Plugin initialization failed. Incorrect 'InTextStylePresets' arg"));
		bSuccess = false;
	}
	else
	{
		TextStylePresets = InTextStylePresets;
	}
	
	if (InTextAnimPresets.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Plugin initialization failed. Incorrect 'InTextAnimPresets' arg"));
		bSuccess = false;
	}
	else
	{
		NarrationDelaysPreset = InTextAnimPresets;
	}

	if (InMasterNarrationDT.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Plugin initialization failed. Incorrect 'InMasterNarrationDT' arg"));
		bSuccess = false;
	}
	else
	{
		MasterNarrationDT = InMasterNarrationDT;
	}
	
	if (InWidgetClass.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Plugin initialization failed. Incorrect 'InWidgetClass' arg"));
		bSuccess = false;
	}
	else
	{
		WidgetClass = InWidgetClass;
	}

	if (InSoundAttenuation.IsNull())
	{
		UE_LOG(LogTemp,Error, TEXT("Plugin initialization failed. Incorrect 'InSoundAttenuation' arg"));
		bSuccess = false;
	}
	else
	{
		SoundAttenuation = InSoundAttenuation;
	}
	
	return bSuccess;
}