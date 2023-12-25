// All rights reserved, HelloIT Norweskie


#include "FunctionLibraries/CheckOutLiteHelpers.h"

bool UCheckOutLiteHelpers::CreateTXT(FString FilePath, FString Content)
{
	if (FFileHelper::SaveStringToFile(Content, *FilePath))
	{
		//Successfully wrote to a file 
		return true;
	}
	else
	{
		//Writing to a file failed 
		return true;
	}
}

bool UCheckOutLiteHelpers::ReadTXT(FString FilePath, FString& Content)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		// File doesnt exist
		Content = "";
		return false;
	}
	
	if (!FFileHelper::LoadFileToString(Content, *FilePath))
	{
		//Could not read from file
		Content = "";
		return false;
	}
	
	return true;
}

