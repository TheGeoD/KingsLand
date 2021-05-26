// Fill out your copyright notice in the Description page of Project Settings.


#include "KLWeapon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "KLProjectile.h"
// Sets default values
AKLWeapon::AKLWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh"); 
	RootComponent = Mesh; 



}

// Called when the game starts or when spawned
void AKLWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void AKLWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
}

bool AKLWeapon::StartUseWeapon()
{
	return true; 
}


