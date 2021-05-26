// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLProjectile.h"
#include "Components/SphereComponent.h"
#include "KLArrow.generated.h"

/**
 * 
 */
UCLASS()
class AKLArrow : public AKLProjectile
{
	GENERATED_BODY()
public: 
	AKLArrow();
private: 
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
