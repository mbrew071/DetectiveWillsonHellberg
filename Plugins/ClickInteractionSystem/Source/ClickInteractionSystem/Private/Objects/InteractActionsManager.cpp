// All rights reserved, HelloIT Norweskie


#include "Objects/InteractActionsManager.h"
#include "Objects/InteractActions.h"

TMap<TSoftClassPtr<UInteractActions>, TSharedPtr<UInteractActions>> UInteractActionsManager::InteractActions;
UInteractActions* UInteractActionsManager::GetInteractActions(TSoftClassPtr<UInteractActions> ActionClass)
{
	if(!ActionClass.IsValid()) {  return nullptr; } 

	TSharedPtr<UInteractActions>* ExistingInstance = InteractActions.Find(ActionClass);
	//TSharedPtr<UInteractActions>* ExistingInstance = InteractActions.Find(ActionClass);
	if (ExistingInstance)
	{
		return ExistingInstance->Get();
	}
	// Create the instance if it doesn't exist
	// Consider "RF_MarkAsRootSet" if I dont want to get garbage collected
	TSharedPtr<UInteractActions> NewInstance = MakeShareable(NewObject<UInteractActions>(GetTransientPackage(), NAME_None));
	InteractActions.Add(ActionClass, NewInstance);
	
	return NewInstance.Get();
}
