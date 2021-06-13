// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/KLInteractables.h"


// Add default functionality here for any IKLInteractables functions that are not pure virtual.

void IKLInteractables::Focused(APawn* Interactor)
{

}

void IKLInteractables::InteractedWith()
{

}

void IKLInteractables::EndFocus()
{

}

bool IKLInteractables::IsBeingInteractedWith()
{
	return bIsBeingInteractedWith; 
}
