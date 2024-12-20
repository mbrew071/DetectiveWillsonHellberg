// © 2024 KAG. All rights reserved.


#include "Subsystems/GraphNarrationController.h"

void UGraphNarrationController::InitializePlugin( const TSoftClassPtr<UUserWidget> InScreenWidgetClass, const TSoftClassPtr<UGraphNarrationCustomAction> InActionCustom, const TSoftClassPtr<UUserWidget> InSceneWidgetClass)
{
	if (InScreenWidgetClass.IsNull())
	{
		UE_LOG(LogTemp,Fatal, TEXT("Invalid Widget class at plugin intialization"));
	}
	//TODO add validation. If widget class doesnt implement widget interface then crash game because it wont work anyways
	ScreenWidgetClass = InScreenWidgetClass;

	if (InActionCustom.IsNull())
	{
		UE_LOG(LogTemp,Fatal, TEXT("Invalid CustomAction class at plugin intialization"));
	}
	ActionCustom = InActionCustom;

	if (InSceneWidgetClass.IsNull())
	{
		UE_LOG(LogTemp,Fatal, TEXT("Invalid Widget class at plugin intialization"));
	}
	SceneWidgetClass = InSceneWidgetClass;
}
