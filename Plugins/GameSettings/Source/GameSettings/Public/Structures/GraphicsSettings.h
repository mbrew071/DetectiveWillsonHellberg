// All rights reserved hehe

#pragma once
#include "GenericSettings.h"
#include "Enumerations/ScreenResolutionEnum.h"
#include "GenericPlatform/GenericWindow.h"
#include "GameplayTagContainer.h"
#include "GraphicsSettings.generated.h"

USTRUCT(BlueprintType)
struct FGraphicsSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 OverallScalability;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 ViewDistanceQuality;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 AntiAliasingQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 PostProcessingQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 ShadowQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 GlobalIlluminationQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 ReflectionsQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 TextureQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 VisualEffectsQuality;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 FoliageQuality;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	int32 ShadingQuality;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	FIntPoint ScreenResolution;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	EScreenResolutionEnum ScreenResolutionEnum;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	float MaxFPS;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	bool bVsync;
	
	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	TEnumAsByte<EWindowMode::Type> WindowMode;

	UPROPERTY(BlueprintReadWrite, Category="GameSettings|Graphics")
	FGenericSettings CustomSettings;
	
};
