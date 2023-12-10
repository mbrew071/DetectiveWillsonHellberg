// All rights reserved hehe


#include "ExeTimer.h"

void UExeTimer::StartTimer(const FString& TimerTag)
{
	//Store it in the TMap
	TimerLogs.Add(TimerTag, FPlatformTime::Seconds());
}

void UExeTimer::StopTimer(const UObject* WorldContextObject, const FString& TimerTag, const FString& Note)
{
	//Does this Timestamp exist? if not then return
	if (!TimerLogs.Contains(TimerTag)) {return;}
	
	//Print
	PrintTimerLog(WorldContextObject, TimerTag, Note);
}

void UExeTimer::PrintTimerLog(const UObject* WorldContextObject, const FString& TimerTag, const FString& Note)
{
	const FString Message = BuildLogMessage(WorldContextObject, TimerTag, Note);
	UE_LOG(LogTemp, Log, TEXT(" \n%s\n%s\n%s"), *LogSeparator, *Message, *LogSeparator);
	
	/*	Example Log:
----------------------------------------
[BP_ExampleActor] || ExeTimer || ExampleTag || 2.2134 ms || Note
----------------------------------------
	*/
}

FString UExeTimer::BuildLogMessage(const UObject* WorldContextObject, const FString& TimerTag, const FString& Note)
{
	FString Message = "";
	
	//[BP_ExampleActor]
	if (!IsValid(WorldContextObject))
	{
		Message.Append("[");
		Message.Append(GetNameSafe(WorldContextObject));
		Message.Append("]");
		Message.Append(MsgSeparator);
	}

	//[BP_ExampleActor] ||  ExeTimer || 
	Message.Append(LogTitle);
	Message.Append(MsgSeparator);

	//[BP_ExampleActor] || ExeTimer || ExampleTag || 
	Message.Append(TimerTag);
	Message.Append(MsgSeparator);

	//[BP_ExampleActor] || ExeTimer || ExampleTag || 2.2134 ms ||
	double Duration = FPlatformTime::Seconds() - *TimerLogs.Find(TimerTag);
	Duration = Duration * 1000; // Convert seconds to ms
	const FString StringDuration = FString::Printf(TEXT("%.5f"), Duration);
	Message.Append(StringDuration);
	Message.Append(" ms");
	Message.Append(MsgSeparator);

	//[BP_ExampleActor] || ExeTimer || ExampleTag || 2.2134 ms || Note
	if (!Note.IsEmpty())
	{
		Message.Append(Note);
	}
	
	return Message;
}
