// Fill out your copyright notice in the Description page of Project Settings.


#include "KLBow.h"
#include "Kismet/GameplayStatics.h"
#include "KLProjectile.h"

AKLBow::AKLBow()
{
	SpeedPerSecondOfDraw = 1000.f; 
	MaxArrowSpeed = 2000.f; 
	ArrowReleaseSocket = FName("ReleaseSocket"); 

	BowTimer = 3.f;
}

bool AKLBow::StartUseWeapon()
{
	if(!((GetWorld()->TimeSeconds - LastFired) > BowTimer))
	{
		return false; 
	}
	StartDrawTime = GetWorld()->TimeSeconds; 
	UE_LOG(LogTemp, Warning, TEXT("%f"), StartDrawTime);
	return true; 
}

void AKLBow::ReleaseBow(FRotator ReleaseAngle)
{
	UWorld* World = GetWorld(); 
	APawn* Firer = Cast<APawn>(GetOwner()); 
	if (Firer == nullptr || Mesh == nullptr || World == nullptr || ArrowClass == nullptr)
	{
		return;
	}
	float TimeSeconds = World->TimeSeconds; 
	float TimeDrawn = TimeSeconds - StartDrawTime; 
	float ReleaseSpeed = TimeDrawn * SpeedPerSecondOfDraw; 
	FTransform SpawnTransform; 
	SpawnTransform.SetLocation(Mesh->GetSocketLocation(ArrowReleaseSocket)); 
	SpawnTransform.SetRotation(ReleaseAngle.Quaternion()); 
	AKLProjectile* SpawningProjectile = World->SpawnActorDeferred<AKLProjectile>(ArrowClass, SpawnTransform, GetOwner(), Firer, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if(SpawningProjectile != nullptr)
	{
		float LaunchSpeed = (GetWorld()->TimeSeconds - StartDrawTime) * SpeedPerSecondOfDraw;
		LaunchSpeed = FMath::Clamp(LaunchSpeed, 0.f, MaxArrowSpeed); 
		SpawningProjectile->InitializeRelease(LaunchSpeed, Firer->GetController(), {this, GetOwner()});

		UGameplayStatics::FinishSpawningActor(SpawningProjectile, SpawnTransform);

		LastFired = TimeSeconds; 
	}
}
