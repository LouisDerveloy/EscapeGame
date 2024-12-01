// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractiveInterface
{
	GENERATED_BODY()
public:
	// Interact function
	virtual void OnInteract(APawn* Sender);
	
	bool ShouldExecOnServer() const { return bShouldExecOnServer; }
	void setShouldExecOnServer(bool value) { bShouldExecOnServer = value; }

private:
	bool bShouldExecOnServer = true;

};
