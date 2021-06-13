// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/KLInteractableTest.h"

// Sets default values
AKLInteractableTest::AKLInteractableTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKLInteractableTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKLInteractableTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKLInteractableTest::Focused(APawn* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("Focused")); 
}

void AKLInteractableTest::InteractedWith()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted"));

}

void AKLInteractableTest::EndFocus()
{
	UE_LOG(LogTemp, Warning, TEXT("End Focus")); 
}
