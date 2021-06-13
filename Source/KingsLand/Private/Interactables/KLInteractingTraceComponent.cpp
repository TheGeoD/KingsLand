// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h" 
#include "KLInteractables.h"
#include "Interactables/KLInteractingTraceComponent.h"


// Sets default values for this component's properties
UKLInteractingTraceComponent::UKLInteractingTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKLInteractingTraceComponent::BeginPlay()
{
	Super::BeginPlay(); 
	if(CameraComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera must be set in the constructor of the owning Parent Actor")); 
	}
}


// Called every frame
void UKLInteractingTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FMinimalViewInfo CameraViewInfo;
		CameraComp->GetCameraView(GetWorld()->DeltaTimeSeconds, CameraViewInfo);
		FRotator CameraRotation = CameraViewInfo.Rotation;
		FVector CameraLocation = CameraViewInfo.Location;
		FVector LTEnd = CameraLocation + (CameraRotation.Vector() * 500.f);
		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, CameraLocation, LTEnd, ECC_Visibility))
		{
			IKLInteractables* InteractableActor = Cast<IKLInteractables>(Hit.Actor);
			if (InteractableActor != nullptr && FocusedItem != InteractableActor)
			{
				if (FocusedItem == nullptr || !FocusedItem->IsBeingInteractedWith())
				{
					FocusedItem = InteractableActor;
					FocusedItem->bIsFocused = true;
					APawn* OwningPawn = Cast<APawn>(GetOwner()); 
					FocusedItem->Focused(OwningPawn);
				}
			}
			else if (FocusedItem != InteractableActor)
			{
				if (FocusedItem != nullptr && !FocusedItem->IsBeingInteractedWith())
				{
					FocusedItem->bIsFocused = false;
					FocusedItem->EndFocus();
					FocusedItem = nullptr;
				}
			}
		}
	}
}

void UKLInteractingTraceComponent::InteractWith()
{
	if (FocusedItem != nullptr)
	{
		FocusedItem->InteractedWith();
	}
}
