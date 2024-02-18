// All rights reserved

#include "FunctionLibraries/CheckOutLiteHelpers.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"


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

bool UCheckOutLiteHelpers::PushAllCommitsToOrigin()
{
	FString GitBinaryPath = TEXT("git"); // Replace with full path if git is not in system path
	FString RepoPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()); // Path to your project repo
	FString Command = TEXT("push origin HEAD"); // Git push command

	// Create full command line
	FString FullCommandLine = FString::Printf(TEXT("-C \"%s\" %s"), *RepoPath, *Command);

	// Create a process to execute the command
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(*GitBinaryPath, *FullCommandLine, true, false, false, nullptr, 0, nullptr, nullptr);

	if (ProcHandle.IsValid())
	{
		// Wait for the process to complete
		FPlatformProcess::WaitForProc(ProcHandle);

		// Optionally, you can check the exit code to determine if the push was successful
		int32 ReturnCode;
		FPlatformProcess::GetProcReturnCode(ProcHandle, &ReturnCode);

		return ReturnCode == 0; // Assuming 0 is the success code
	}
	return false;
}

