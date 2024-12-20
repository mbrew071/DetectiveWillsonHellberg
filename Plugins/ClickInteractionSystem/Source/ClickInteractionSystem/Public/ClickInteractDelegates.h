// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClickInteractDelegates.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionBegin, AActor*, InteractionActor);
/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionProgress, const int32, Progress);*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionEnd, const bool, bSuccess);

USTRUCT()
struct FClickInteractDelegates
{
	GENERATED_BODY();
};