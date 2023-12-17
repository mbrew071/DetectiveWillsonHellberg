// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Item_I.h"
#include "Structures/ItemData.h"
#include "Item.generated.h"

UCLASS(Abstract)
class GAMEPROJECT_API AItem : public AActor, public IItem_I
{
	GENERATED_BODY()

	///////////////////////////////////Default////////////////////////////////////////////////////////////////
public:	
	AItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////////////////////////////////////Item Data//////////////////////////////////////////////////////
public:
	//Data for this item 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Design|ItemData")
	FItemData ItemData;

	////////////////////////////////////////General///////////////////////////////////////////////
public:
	//Root
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent;
	
private:
	// Initializes root
	void InitSceneComponent();

	//Initializes tags for that actor
	void InitTags();
	
};
