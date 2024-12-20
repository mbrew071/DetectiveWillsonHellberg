// © 2024 KAG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structures/GraphNarrationVariables.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NarrationVariablesContainer.generated.h"

/**
 * This is a container for all Simple Narration Variables that need to be protected from eg
 * A) Destruction during level change
 * B) Destruction after dialogue ends 
 */
UCLASS(DisplayName="Narration Variables")
class SIMPLENARRATION_API UNarrationVariablesContainer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

/////////////////////////////////////// ACTOR VARIABLES /////////////////////////////////////////////
private:	
	//This is Map of variables per an NPC scope. Key is npc FName
	UPROPERTY(SaveGame)
	TMap<FName,FGraphNarrationVariables> NarrationVariablesNPC;

public:	
//Booleans:
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, Scope"))
	void SetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, const bool Value ); 
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, bCreateIfNotFound, Scope"))
	void GetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, bool& Value, const bool bCreateIfNotFound);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, Scope"))
	void RemoveNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName);

//Ints
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, Scope"))
	void SetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, const int32 Value);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, bCreateIfNotFound, Scope"))
	void GetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="NPC, Scope"))
	void RemoveNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName);

///////////////////////////////////// GLOBAL VARIABLES////////////////////////////////////////////
private:
	//This is map of variables per a Group of NPCs, Key is group tag
	UPROPERTY(SaveGame)
	TMap<FGameplayTag,FGraphNarrationVariables> NarrationVariablesGroup; 

public:
// Booleans
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="Scope"))
	void SetNarrationBoolGlobal(const FName VariableName, const bool Value, const FGameplayTag Scope);
	
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay=" bCreateIfNotFound, Scope"))
	void GetNarrationBoolGlobal(const FName VariableName, bool& bSuccess, bool& Value, const FGameplayTag Scope, const bool bCreateIfNotFound);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="Scope"))
	void RemoveNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope);

//Ints
	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="Scope"))
	void SetNarrationIntGlobal(const FName VariableName, const int32 Value, const FGameplayTag Scope);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="bCreateIfNotFound, Scope"))
	void GetNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound);

	UFUNCTION(BlueprintCallable, Category="SimpleNarration|Variables", meta=(AdvancedDisplay="Scope"))
	void RemoveNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope);

////////////////////////////////////// HELPERS /////////////////////////////////////////
private:
	FGameplayTag ValidateScope(const FGameplayTag Scope);
};
