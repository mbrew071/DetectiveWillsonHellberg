// All rights reserved hehe

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestLoop.generated.h"

UCLASS()
class ACTORNARRATION_API ATestLoop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestLoop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	float Delay = 1.0f;
	
	void StartLoopWithLambda();
	void DelayLoop();
};
