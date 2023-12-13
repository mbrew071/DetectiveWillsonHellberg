// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Structures/Items.h"
#include "Inventory.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//////////////////////////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<TSoftClassPtr<AActor>, int32> Inventory;

	///////////////////////////////////////////////////////////////////////////////////////////
public:
	// e chyba nie potrzebujemy takich funkcji, normalnie na mapie movemy operowac w sumie
//	UFUNCTION(BlueprintCallable)
//	bool AddItem(const TSoftClassPtr<AActor> Class, FItem Item);

//	UFUNCTION(BlueprintCallable)
//	bool RemoveItem(const TSoftClassPtr<AActor> Class, FItem Item);

//	UFUNCTION(BlueprintCallable)
//	bool FindItem(const TSoftClassPtr<AActor> Class, FItem Item);
};
