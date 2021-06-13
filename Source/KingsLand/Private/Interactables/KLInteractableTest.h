// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KLInteractables.h"
#include "KLInteractableTest.generated.h"

UCLASS()
class AKLInteractableTest : public AActor, public IKLInteractables
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKLInteractableTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Focused(APawn* Interactor) override; 

	virtual void InteractedWith() override; 

	virtual void EndFocus() override; 

};
