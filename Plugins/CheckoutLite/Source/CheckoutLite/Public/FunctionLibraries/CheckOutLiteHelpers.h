// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheckOutLiteHelpers.generated.h"


UCLASS()
class CHECKOUTLITE_API UCheckOutLiteHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Creates a text file with string content. If the file already exists it gets replaced.
	*
	* @param FilePath	The path of the file
	* @param Content	String content that will be written to this file
	* @return success or failure of the process
	*/
	UFUNCTION(BlueprintCallable, Category = "")
	static bool CreateTXT(FString FilePath, FString Content);
	
	/**
	 * Reads a string content of a text file
	 *
	 * @param FilePath	The path of the file
	 * @param Content	Text content of the file
	 * @return success or failure of the process
	 */
	UFUNCTION(BlueprintCallable, Category = "CheckOutLite|ReadFile")
	static bool ReadTXT(FString FilePath, FString& Content);
	
};
