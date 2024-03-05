// All rights reserved hehe


#include "TestLoop.h"

// Sets default values
ATestLoop::ATestLoop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestLoop::BeginPlay()
{
	Super::BeginPlay();
	//StartLoopWithLambda();
	DelayLoop();
}

// Called every frame
void ATestLoop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/////////////////////// With Lambda

void ATestLoop::StartLoopWithLambda()
{
	// Use a lambda function for the loop logic
	auto LoopLambda = [this]()
	{
		// Code to be executed
		UE_LOG(LogTemp, Warning, TEXT("Looping with Lambda..."));
	};

	// Set up the initial timer with the lambda function
	GetWorldTimerManager().SetTimer(TimerHandle, LoopLambda, 1.0f, true);
}

////////////////////////// With Function
void ATestLoop::DelayLoop()
{
	Delay = Delay +0.25f;
	UE_LOG(LogTemp, Display, TEXT(" Delay is %f"), Delay);
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestLoop::DelayLoop, Delay, false);
}