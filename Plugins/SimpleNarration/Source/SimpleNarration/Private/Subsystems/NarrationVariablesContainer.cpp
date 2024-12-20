// © 2024 KAG. All rights reserved.

#include "Subsystems/NarrationVariablesContainer.h"
#include "GameFramework/Actor.h"

/////////////////////////////// Global narration variables ////////////////////////////////////////

void UNarrationVariablesContainer::SetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, const bool Value)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to set variable for NPC but valid NPC was not provided"));
		return;
	}
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source = &NarrationVariablesNPC.FindOrAdd(FName(SoftClassPtr.GetAssetName()));
	if (Source)
	{
		Source->Booleans.Add(VariableName, Value);	
	}
}

void UNarrationVariablesContainer::GetNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, bool& Value, const bool bCreateIfNotFound)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to get variable for NPC but valid NPC was not provided"));
		return;
	}
	
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source;
	if(bCreateIfNotFound)
	{
		Source = &NarrationVariablesNPC.FindOrAdd(FName(SoftClassPtr.GetAssetName()));
	}
	else
	{
		Source = NarrationVariablesNPC.Find(FName(SoftClassPtr.GetAssetName()));
	}
	
	if(Source == nullptr)
	{
		bSuccess = false;
		return;
	}
	
	if (bCreateIfNotFound)
	{
		bool Temp = Source->Booleans.FindOrAdd(VariableName);
		Value = Temp;
		bSuccess = true;
		return;
	}
	bool* bTemp = Source->Booleans.Find(VariableName);
	if (bTemp)
	{
		Value = *bTemp;
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

void UNarrationVariablesContainer::RemoveNarrationBool(UPARAM(ref) AActor* Actor, const FName VariableName)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to remove variable for NPC but valid NPC was not provided"));
		return;
	}
	
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source = NarrationVariablesNPC.Find(FName(SoftClassPtr.GetAssetName()));
	if (Source)
	{
		Source->Booleans.Remove(VariableName);
	}
}

void UNarrationVariablesContainer::SetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, const int32 Value)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to set variable for NPC but valid NPC was not provided"));
		return;
	}
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source = &NarrationVariablesNPC.FindOrAdd(FName(SoftClassPtr.GetAssetName()));
	if (Source)
	{
		Source->Ints.Add(VariableName, Value);	
	}
}

void UNarrationVariablesContainer::GetNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to get variable for NPC but valid NPC was not provided"));
		return;
	}
	
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source;
	if(bCreateIfNotFound)
	{
		Source = &NarrationVariablesNPC.FindOrAdd(FName(SoftClassPtr.GetAssetName()));
	}
	else
	{
		Source = NarrationVariablesNPC.Find(FName(SoftClassPtr.GetAssetName()));
	}
	
	if(Source == nullptr)
	{
		bSuccess = false;
		return;
	}
	
	if (bCreateIfNotFound)
	{
		int32 Temp = Source->Ints.FindOrAdd(VariableName);
		Value = Temp;
		bSuccess = true;
		return;
	}
	int32* Temp = Source->Ints.Find(VariableName);
	if (Temp)
	{
		Value = *Temp;
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

void UNarrationVariablesContainer::RemoveNarrationInt(UPARAM(ref) AActor* Actor, const FName VariableName)
{
	if (Actor==nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to remove variable for NPC but valid NPC was not provided"));
		return;
	}
	
	const TSoftClassPtr<UObject> SoftClassPtr = Actor->GetClass();
	FGraphNarrationVariables* Source = NarrationVariablesNPC.Find(FName(SoftClassPtr.GetAssetName()));
	if (Source)
	{
		Source->Ints.Remove(VariableName);
	}
}

/// Booleans
void UNarrationVariablesContainer::SetNarrationBoolGlobal(const FName VariableName, const bool Value, const FGameplayTag Scope)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source = &NarrationVariablesGroup.FindOrAdd(ValidScope);
	if (Source)
	{
		Source->Booleans.Add(VariableName, Value);	
	}
}

void UNarrationVariablesContainer::GetNarrationBoolGlobal(const FName VariableName, bool& bSuccess, bool& Value, const FGameplayTag Scope, const bool bCreateIfNotFound)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source;
	if(bCreateIfNotFound)
	{
		Source = &NarrationVariablesGroup.FindOrAdd(ValidScope);
	}
	else
	{
		Source = NarrationVariablesGroup.Find(ValidScope);
	}
	
	if(Source == nullptr)
	{
		bSuccess = false;
		return;
	}
	
	if (bCreateIfNotFound)
	{
		bool Temp = Source->Booleans.FindOrAdd(VariableName);
		Value = Temp;
		bSuccess = true;
		return;
	}
	bool* bTemp = Source->Booleans.Find(VariableName);
	if (bTemp)
	{
		Value = *bTemp;
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

void UNarrationVariablesContainer::RemoveNarrationBoolGlobal(const FName VariableName, const FGameplayTag Scope)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source = NarrationVariablesGroup.Find(ValidScope);
	if (Source)
	{
		Source->Booleans.Remove(VariableName);
	}
}

//// Ints
void UNarrationVariablesContainer::SetNarrationIntGlobal(const FName VariableName, const int32 Value, const FGameplayTag Scope)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source = &NarrationVariablesGroup.FindOrAdd(ValidScope);
	if (Source)
	{
		Source->Ints.Add(VariableName, Value);	
	}
}

void UNarrationVariablesContainer::GetNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope, bool& bSuccess, int32& Value, const bool bCreateIfNotFound)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source;
	if(bCreateIfNotFound)
	{
		Source = &NarrationVariablesGroup.FindOrAdd(ValidScope);
	}
	else
	{
		Source = NarrationVariablesGroup.Find(ValidScope);
	}
	
	if(Source == nullptr)
	{
		bSuccess = false;
		return;
	}
	
	if (bCreateIfNotFound)
	{
		int32 Temp = Source->Ints.FindOrAdd(VariableName);
		Value = Temp;
		bSuccess = true;
		return;
	}
	int32* Temp = Source->Ints.Find(VariableName);
	if (Temp)
	{
		Value = *Temp;
		bSuccess = true;
		return;
	}
	bSuccess = false;
}

void UNarrationVariablesContainer::RemoveNarrationIntGlobal(const FName VariableName, const FGameplayTag Scope)
{
	const FGameplayTag ValidScope = ValidateScope(Scope);
	
	FGraphNarrationVariables* Source = NarrationVariablesGroup.Find(ValidScope);
	if (Source)
	{
		Source->Ints.Remove(VariableName);
	}
}

FGameplayTag UNarrationVariablesContainer::ValidateScope(const FGameplayTag Scope)
{
	if (Scope == FGameplayTag())
	{
		return FGameplayTag::RequestGameplayTag("GraphNarration.VariableScope.Default");
	}
	return Scope;
}

