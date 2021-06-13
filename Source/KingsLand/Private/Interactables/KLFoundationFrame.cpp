// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/KLFoundationFrame.h"
#include "DrawDebugHelpers.h"

AKLFoundationFrame::AKLFoundationFrame()
{
	PrimaryActorTick.bCanEverTick = true;

	SocketTypeToMatch = "Foundation"; 
	SetAtTerrainSocketKey = "SetAtTerrain"; 
	HeightOfFoundationPlacement = 200.f; 
}

void AKLFoundationFrame::BeginPlay()
{
	Super::BeginPlay(); 
	TArray<FName> SocketNames = MeshComp->GetAllSocketNames();
	for(FName SName : SocketNames)
	{
		if(SName.ToString().Contains(SetAtTerrainSocketKey))
		{
			SetAtTerrainSockets.Add(SName); 
		}
	}
}

void AKLFoundationFrame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool bCanPlaceLocal = true;
	for (FName SocketN : SetAtTerrainSockets)
	{
		FVector StartTrace = MeshComp->GetSocketLocation(SocketN);
		FVector EndTrace = StartTrace - (StartTrace.UpVector * HeightOfFoundationPlacement);
		FHitResult Hit;
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, 0.f, false); 
		if (!GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility))
		{
			bCanPlaceLocal = false; 
		}

	}
	bCanPlace = bCanPlaceLocal; 
}

void AKLFoundationFrame::InteractedWith()
{
	Super::InteractedWith();
}

void AKLFoundationFrame::AdjustRotation(FTransform TransformAdjustments, FRotator CurrentSocketRotation)
{
	float NewYawRotation = (MeshComp->GetComponentRotation().Yaw - CurrentSocketRotation.Yaw) + (TransformAdjustments.Rotator().Yaw - 180.f);
	FRotator NewRotation = MeshComp->GetComponentRotation(); 
	NewRotation.Yaw = NewYawRotation; 
	MeshComp->SetWorldRotation(NewRotation);
}
