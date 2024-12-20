// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GraphNarrationController.generated.h"

/**
 * 
 */
class UGraphNarrationCustomAction;

UCLASS()
class SIMPLENARRATION_API UGraphNarrationController : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="SimpleNarration")
	void InitializePlugin(const TSoftClassPtr<UUserWidget> InScreenWidgetClass, const TSoftClassPtr<UGraphNarrationCustomAction> InActionCustom, const TSoftClassPtr<UUserWidget> InSceneWidgetClass);

public:
	//Class where we implement behaviour for all custom actions
	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	TSoftClassPtr<UGraphNarrationCustomAction> ActionCustom;

	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	TSoftClassPtr<UUserWidget> ScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="GraphNarration")
	TSoftClassPtr<UUserWidget> SceneWidgetClass;
	
};


