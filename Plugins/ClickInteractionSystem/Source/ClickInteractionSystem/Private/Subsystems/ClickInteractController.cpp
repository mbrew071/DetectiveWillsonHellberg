// All rights reserved KAG 2024


#include "Subsystems/ClickInteractController.h"
//#include "Objects/InteractActions.h"

bool UClickInteractController::InitializePlugin( TSoftClassPtr<UUserWidget> InDefaultWidget, FGameplayTag InRangeArea, FGameplayTag InOverlapTarget) 
{
	bool bSuccess = true;

	/*if (InInteractActions.IsNull())
	{
		bSuccess = false;
		UE_LOG(LogTemp, Fatal, TEXT("Failed to initialize plugin. InDefaultWidget is invalid"))
	}
	else
	{
		InteractActions = InInteractActions;
	}*/

	if (InDefaultWidget.IsNull())
	{
		bSuccess = false;
		UE_LOG(LogTemp, Fatal, TEXT("Failed to initialize plugin. InDefaultWidget is invalid"))
	}
	else
	{
		DefaultWidget = InDefaultWidget;
	}

	/*if (InInteractComponent.IsValid())
	{
		InteractComponent = InInteractComponent;
	}
	else
	{
		bSuccess = false;
		UE_LOG(LogTemp, Fatal, TEXT("Failed to initialize plugin. InInteractComponent is invalid"))
	}*/

	if (InRangeArea.IsValid())
	{
		RangeArea = InRangeArea;
	}
	else
	{
		bSuccess = false;
		UE_LOG(LogTemp, Fatal, TEXT("Failed to initialize plugin. InRangeArea is invalid"))
	}

	if (InOverlapTarget.IsValid())
	{
		OverlapTarget = InOverlapTarget;
	}
	else
	{
		bSuccess = false;
		UE_LOG(LogTemp, Fatal, TEXT("Failed to initialize plugin. InOverlapTarget is invalid"))
	}
	return bSuccess;
}
