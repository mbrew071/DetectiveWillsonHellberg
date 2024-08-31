// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DetectiveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UDetectiveGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void Start();
	
private:	
	virtual void OnStart() override;
};
