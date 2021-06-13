// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/KLConstructionActor.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AKLConstructionActor::AKLConstructionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp"); 
	MeshComp->SetSimulatePhysics(true); 
	RootComponent = MeshComp; 

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox"); 
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapBox->SetupAttachment(RootComponent); 
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AKLConstructionActor::BeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AKLConstructionActor::EndOverlap);

	bIsPlaced = false; 
	bIsOverlappingWithCompatableActor = false; 

	AttachSocketKey = "Attach";
}

// Called when the game starts or when spawned
void AKLConstructionActor::BeginPlay()
{
	Super::BeginPlay();
	TArray<FName> SocketNames = MeshComp->GetAllSocketNames();
	for (FName SName : SocketNames)
	{
		if (SName.ToString().Contains(AttachSocketKey))
		{
			AttachSockets.Add(SName);
		}
	}
}


// Called every frame
void AKLConstructionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsBeingInteractedWith)
	{
		FRotator ObjectRotation = MeshComp->GetComponentRotation();
		ObjectRotation.Roll = 0.f;
		ObjectRotation.Pitch = 0.f;
		MeshComp->SetWorldRotation(ObjectRotation);

		FindAttachements(); 
	}
}

void AKLConstructionActor::FindAttachements()
{
	if (bIsOverlappingWithCompatableActor && CompatableActor != nullptr && CompatableActor->IsPlaced() && !bSnappedToActor)
	{
		TArray<FTransform> SocketTransformList = CompatableActor->GetAttachPointSockets(SocketTypeToMatch);
		if(SocketTransformList.Num() <= 0)
		{
			return; 
		}

		float ShortestDistance = (MeshComp->GetSocketTransform(RelevantSockets[0]).GetLocation() - SocketTransformList[0].GetLocation()).Size();
		FName SocketName = RelevantSockets[0]; 
		FTransform CompatableSocketTransform = SocketTransformList[0]; 
		SecondSocketTransform = SocketTransformList[0]; 
		UE_LOG(LogTemp, Warning, TEXT("----------------------------------------------------"));
		for (FName RelevantSocket : RelevantSockets)
		{
			for (FTransform SocketTransform : SocketTransformList)
			{
				float CurrentDistance = (SocketTransform.GetLocation() - MeshComp->GetSocketTransform(RelevantSocket).GetLocation()).Size();
				UE_LOG(LogTemp, Warning, TEXT("All Distances: %s"), *RelevantSocket.ToString());
				if (CurrentDistance < ShortestDistance)
				{
					SocketName = RelevantSocket;
					CompatableSocketTransform = SocketTransform;
					ShortestDistance = CurrentDistance;
					SecondSocketTransform = SocketTransform; 
				}
			}
		}

		SnapToCompatableComponent(SocketName, CompatableSocketTransform);

		bool bPersistant = true; 
		FVector LineStart1 = MeshComp->GetSocketLocation(SocketName); 
		DrawDebugSphere(GetWorld(), LineStart1, 50.f, 24, FColor::Red, bPersistant, 0.f);
		DrawDebugSphere(GetWorld(), SecondSocketTransform.GetLocation(), 50.f, 24, FColor::Red, bPersistant, 0.f);

		FVector LineEnd1 = LineStart1 + MeshComp->GetSocketRotation(SocketName).Vector() * 200.f; 
		FVector LineEnd2 = SecondSocketTransform.GetLocation() + (SecondSocketTransform.Rotator().Vector() * 200.f);
		DrawDebugDirectionalArrow(GetWorld(), LineStart1, LineEnd1, 25.f, FColor::Blue, bPersistant, 0.f);
		DrawDebugDirectionalArrow(GetWorld(), SecondSocketTransform.GetLocation(), LineEnd2, 25.f, FColor::Blue, bPersistant, 0.f);

		FVector LineStart3 = MeshComp->GetComponentLocation(); 
		FVector LineEnd3 = LineStart3 + (MeshComp->GetComponentRotation().Vector() * 200.f); 
		DrawDebugDirectionalArrow(GetWorld(), LineStart3, LineEnd3, 25.f, FColor::Blue, bPersistant, 0.f);

		FVector LineStart4 = CompatableActor->GetActorLocation();
		FVector LineEnd4 = LineStart4 + (CompatableActor->GetActorRotation().Vector() * 200.f);
		DrawDebugDirectionalArrow(GetWorld(), LineStart3, LineEnd3, 25.f, FColor::Blue, bPersistant, 0.f);

	}
}

void AKLConstructionActor::AdjustRotation(FTransform TransformAdjustments, FRotator CurrentSocketRotation)
{
	SetActorRotation(TransformAdjustments.Rotator()); 
}

void AKLConstructionActor::SnapToCompatableComponent(FName Check1, FTransform Check2)
{
	if(Check1.IsEqual(Check1) || Check2.GetLocation() != SecondSocketTransform.GetLocation())
	{
		AdjustRotation(Check2, MeshComp->GetSocketRotation(Check1));
		FVector NewLocation = Check2.GetLocation() + (MeshComp->GetComponentLocation() - MeshComp->GetSocketLocation(Check1));
		MeshComp->SetWorldLocation(NewLocation);
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform; 
		AttachToActor(CompatableActor, AttachmentRules); 
		bSnappedToActor = true; 
	}
}

void AKLConstructionActor::Focused(APawn* Interactor)
{
	FocusingPawn = Interactor; 
}

void AKLConstructionActor::InteractedWith()
{
	if(bIsPlaced)
	{
		return; 
	}
	UCameraComponent* Camera = Cast<UCameraComponent>(FocusingPawn->GetComponentByClass(InteractorCamera));
	if (Camera != nullptr && !bIsBeingInteractedWith)
	{
		MeshComp->SetSimulatePhysics(false);
		FAttachmentTransformRules AttachRule = FAttachmentTransformRules::KeepWorldTransform;
		AttachRule.RotationRule = EAttachmentRule::SnapToTarget;
		AttachToComponent(Camera, AttachRule);
		FVector FocusLocation = Camera->GetComponentLocation() + (Camera->GetComponentRotation().Vector() * 500.f);
		SetActorLocation(FocusLocation); 
		bIsBeingInteractedWith = true; 
		SetOwner(FocusingPawn);
	}
	else if (bIsBeingInteractedWith)
	{
		bIsBeingInteractedWith = false; 
		MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
		FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform; 
		DetachFromActor(DetachRules);
		if(!bCanPlace)
		{
			MeshComp->SetSimulatePhysics(true);
		}
		else
		{
			bIsPlaced = true;
		}
	}
}

void AKLConstructionActor::EndFocus()
{

}

void AKLConstructionActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CompatableActor = Cast<AKLConstructionActor>(OtherActor);
	if(CompatableActor != nullptr)
	{
		bIsOverlappingWithCompatableActor = true; 
		FString MatchingName = CompatableActor->GetSocketTypeToMatch();
		for(FName SocketName : AttachSockets)
		{
			if (SocketName.ToString().Contains(MatchingName))
			{
				RelevantSockets.Add(SocketName);
			}
		}
	}
}

TArray<FTransform> AKLConstructionActor::GetAttachPointSockets(FString AttachSocketType)
{
	TArray<FTransform> SocketTransforms; 
	if (AttachSockets.Num() > 0.f)
	{
		for (FName MeshSocket : AttachSockets)
		{
			if (MeshSocket.ToString().Contains(AttachSocketType))
			{
				SocketTransforms.Add(MeshComp->GetSocketTransform(MeshSocket));
			}
		}
	}
	else if(SocketTransforms.Num() <= 0.f)
	{
		FTransform ZeroTransform; 
		SocketTransforms.Add(ZeroTransform); 
	}
	return SocketTransforms; 
}

bool AKLConstructionActor::IsPlaced()
{
	return bIsPlaced; 
}

void AKLConstructionActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlappingWithCompatableActor = false; 
	RelevantSockets.Empty(); 
}

FString AKLConstructionActor::GetSocketTypeToMatch()
{
	return SocketTypeToMatch;
}