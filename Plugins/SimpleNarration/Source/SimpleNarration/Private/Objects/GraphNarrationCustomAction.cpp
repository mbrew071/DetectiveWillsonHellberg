
#include "Objects/GraphNarrationCustomAction.h"
#include "Engine/Level.h"
#include "Engine/World.h"

UWorld* UGraphNarrationCustomAction::GetWorld() const
{
	// CDO objects do not belong to a world
	// If the actors outer is destroyed or unreachable we are shutting down and the world should be nullptr
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("Actor: %s has a null OuterPrivate in AActor::GetWorld()"), *GetFullName()) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (ULevel* Level = GetTypedOuter<ULevel>())
		{
			return Level->OwningWorld;
		}
	}

	UObject* Outer = GetOuter();
	if (UWorld* World = Cast<UWorld>(Outer))
	{
		return World;
	}
	/*
	if (AActor* Actor = Cast<AActor>(Outer))
	{
		return Actor->GetWorld();
	}
	if (UActorComponent* ActorComponent = Cast<UActorComponent>(Outer))
	{
		return ActorComponent->GetOwner()->GetWorld();
	}
	if (ULevel* Level = Cast<ULevel>(Outer))
	{
		return Level->OwningWorld;
	}*/
	return nullptr;
}

void UGraphNarrationCustomAction::PerformCustomAction_Implementation(const UObject* Graph, const UObject* Widget, const FGameplayTag ActionTag, const UObject* DTO)
{
	OnInteractionBegin.Broadcast();

	//Switch on action type...
}
