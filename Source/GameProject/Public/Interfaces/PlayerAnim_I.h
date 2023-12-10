// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerAnim_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerAnim_I : public UInterface
{
	GENERATED_BODY()
};

class GAMEPROJECT_API IPlayerAnim_I
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCrouchState(const bool bNewState);

	//False -> blendspace 1d
	//True -> blendspace 3d
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBlendspace(const bool bState);
};
