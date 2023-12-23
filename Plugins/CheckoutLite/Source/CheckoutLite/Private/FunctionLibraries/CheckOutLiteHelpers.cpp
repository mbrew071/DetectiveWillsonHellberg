// All rights reserved, HelloIT Norweskie


#include "FunctionLibraries/CheckOutLiteHelpers.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Structures/CheckOutSheet.h"

void UCheckOutLiteHelpers::RemoveRowFromDataTable(UDataTable* DataTable, FName RowName)
{
	// Row found, remove it
	DataTable->RemoveRow(RowName);
	UE_LOG(LogTemp, Warning, TEXT("Row '%s' removed from DataTable."), *RowName.ToString());

	// Update the asset registry to reflect the changes
	FAssetRegistryModule::AssetCreated(DataTable);
	FAssetRegistryModule::AssetRenamed(DataTable, DataTable->GetPathName());
	
}
