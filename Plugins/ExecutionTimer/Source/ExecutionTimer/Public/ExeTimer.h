// All rights reserved hehe

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "ExeTimer.generated.h"

UCLASS()
class EXECUTIONTIMER_API UExeTimer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	/*
	 * Starts the counting for provided TimerTag
	 */
	UFUNCTION(BlueprintCallable, Category="TriggerTimer", meta=(Keywords="Begin,Initialize"))
	void StartTimer(const FString& TimerTag);

	/*
	 * Prints the "ExeTimer" log
	 * Log contains duration since TimerTag started
	 * Duration is always in milliseconds
	 */
	UFUNCTION(BlueprintCallable, Category="TriggerTimer",
		meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, AdvancedDisplay="Note", Keywords="End,Finish,Pause"))
	void StopTimer( const UObject* WorldContextObject, const FString& TimerTag, const FString& Note);

private:
	
	UPROPERTY()
	TMap<FString, double> TimerLogs;

	// Formatting strings
	UPROPERTY()
	FString LogTitle = "ExeTimer";

	UPROPERTY()
	FString MsgSeparator = " || ";

	UPROPERTY()
	FString LogSeparator = "------------------------------------------------------";

	//Utility Functions
	UFUNCTION()
	void PrintTimerLog( const UObject* WorldContextObject, const FString& TimerTag, const FString& Note);
	
	UFUNCTION()
	FString BuildLogMessage(const UObject* WorldContextObject, const FString& TimerTag, const FString& Note);
};
