// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structures/CheckOutData.h"
#include "CheckOutSheet.generated.h"

/**
 * 
 */
UCLASS()
class CHECKOUTLITE_API UCheckOutSheet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="CheckOutData")
	TMap<FString, FCheckOutData> CheckedOutAssets; 
};
