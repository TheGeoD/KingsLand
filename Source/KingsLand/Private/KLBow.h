// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KLWeapon.h"
#include "KLBow.generated.h"

/**
 * 
 */
class AKLProjectile; 

UCLASS(BlueprintType)
class AKLBow : public AKLWeapon
{
	GENERATED_BODY()
protected:
	AKLBow(); 

	UPROPERTY()
	float StartDrawTime; 

	UPROPERTY()
	FName ArrowReleaseSocket; 

	UPROPERTY(EditDefaultsOnly)
	float SpeedPerSecondOfDraw; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKLProjectile> ArrowClass; 

	UPROPERTY(EditDefaultsOnly)
	float BowTimer;

	float LastFired; 

	UPROPERTY(EditDefaultsOnly)
	float MaxArrowSpeed;
public: 
	bool StartUseWeapon();
	void ReleaseBow(FRotator ReleaseAngle);
};
