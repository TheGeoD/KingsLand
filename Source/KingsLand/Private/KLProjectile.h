// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KLProjectile.generated.h"

class UProjectileMovementComponent; 
class UStaticMeshComponent; 
class USphereComponent; 

UCLASS()
class AKLProjectile : public AActor
{
	GENERATED_BODY()
	
public:	 
	// Sets default values for this actor's properties
	AKLProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionSphere; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh; 

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); 

	float Damage; 

	AController* OwningController; 

public:	

	void InitializeRelease(float Speed, AController* Controller, TArray<AActor*> IgnoringActors); 

};
