// All rights reserved hehe

#pragma once
#include "Engine/DataTable.h"
#include "MasterNarration.generated.h"

USTRUCT(BlueprintType)
struct FMasterNarration : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorNarration|DataTable")
	TSoftObjectPtr<UDataTable> DataTable;
};
