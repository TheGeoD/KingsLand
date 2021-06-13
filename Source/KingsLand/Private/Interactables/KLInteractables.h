// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KLInteractables.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKLInteractables : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IKLInteractables
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	bool bIsFocused = false; 

	APawn* FocusingPawn;

	bool bIsBeingInteractedWith = false; 

	/**
	 * Param Interactor is the Actor that is focused on the intractable object
	 */
	virtual void Focused(APawn* Interactor); 

	virtual void InteractedWith(); 

	virtual void EndFocus(); 

	virtual bool IsBeingInteractedWith(); 

protected:

};
