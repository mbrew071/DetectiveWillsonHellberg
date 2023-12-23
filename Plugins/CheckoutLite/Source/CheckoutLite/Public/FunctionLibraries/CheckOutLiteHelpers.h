// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheckOutLiteHelpers.generated.h"

/**
 * 
 */
UCLASS()
class CHECKOUTLITE_API UCheckOutLiteHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "DataTables")
	static void RemoveRowFromDataTable(UDataTable* DataTable, FName RowName);
};
