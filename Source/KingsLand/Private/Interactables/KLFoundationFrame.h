// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/KLConstructionActor.h"
#include "KLFoundationFrame.generated.h"

/**
 * 
 */
UCLASS()
class AKLFoundationFrame : public AKLConstructionActor
{
	GENERATED_BODY()
	
public: 
	AKLFoundationFrame(); 

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override; 

	virtual void InteractedWith() override;

protected:

	TArray<FName> SetAtTerrainSockets; 

	UPROPERTY(EditDefaultsOnly)
	FString SetAtTerrainSocketKey; 

	UPROPERTY(EditDefaultsOnly)
	float HeightOfFoundationPlacement; 

	virtual void AdjustRotation(FTransform TransformAdjustments, FRotator CurrentSocketRotation) override;
};
