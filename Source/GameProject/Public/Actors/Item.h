// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Item_I.h"
#include "Structures/ItemData.h"
#include "Item.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;
class USphereComponent;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data")
	FItemData ItemData;

	////////////////////////////////////////General///////////////////////////////////////////////
public:
	//Primary collision component for the item
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneComponent;
	
private:
	// Initializes all components, setups attachments
	void InitComponents();

	//Initializes tags for that actor
	void InitTags();
	
	///////////////////////////////////////////Collision//////////////////////////////////////////////////////////
private:
	//Initializes primary collision for item
	void InitCollision();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Collision")
	USphereComponent* SphereComponent;

	///////////////////////////////////////////Widget//////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;

	//Ref to spawned UserWidget
	UPROPERTY()
	UUserWidget* UserWidget;
private:
	UFUNCTION(BlueprintCallable, Category="Widget")
	bool SetWidgetVisibility (const bool bNewVisibility);
public:
	virtual void SetWidgetVisibility_I_Implementation(const bool bNewVisibility) override;
};
