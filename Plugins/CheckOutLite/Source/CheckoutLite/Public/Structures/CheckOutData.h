// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CheckOutData.generated.h"

USTRUCT(BlueprintType)
struct FCheckOutData : public FTableRowBase
{
	GENERATED_BODY()

	//Asset that is checked out by user
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//FString Asset;
	
	//Uset that checked out the asset
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString User;

	//Timestamp of the check out
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Timestamp;
};
