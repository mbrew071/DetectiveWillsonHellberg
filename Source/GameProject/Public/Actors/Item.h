// All rights reserved, HelloIT Norweskie

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structures/ItemData.h"
#include "Item.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS(Abstract)
class GAMEPROJECT_API AItem : public AActor
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
	UWidgetComponent* Widget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Widget")
	TSoftClassPtr<UUserWidget> WidgetClass;

	UFUNCTION(BlueprintCallable, Category="Widget")
	void SetWidgetVisibility (const bool bNewVisibility);
};
