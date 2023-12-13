// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_I.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem_I : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPROJECT_API IItem_I
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWidgetVisibility_I(const bool bNewVisibility);
	

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
