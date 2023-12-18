// All rights reserved, HelloIT Norweskie


#include "Objects/InteractActionsManager.h"
#include "Objects/InteractActions.h"

TMap<TSoftClassPtr<UInteractActions>, UInteractActions*> UInteractActionsManager::InteractActions;
UInteractActions* UInteractActionsManager::GetInteractActions(TSoftClassPtr<UInteractActions> ActionClass)
{
	UClass* ActionClassLoaded = ActionClass.LoadSynchronous();
	//if(!ActionClass.LoadSynchronous()) {  return nullptr; } 
	
	UInteractActions** ExistingInstance = InteractActions.Find(ActionClass);
	if (ExistingInstance)
	{
		return *ExistingInstance;
	}
	
	UInteractActions* NewInstance = NewObject<UInteractActions>(GetTransientPackage(), ActionClass.LoadSynchronous(),
	                                                            NAME_Object);
	InteractActions.Add(ActionClass, NewInstance);
	return *InteractActions.Find(ActionClass);
}
