// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/KLInteractables.h"
#include "KLConstructionActor.generated.h"

class UCameraComponent; 
class UBoxComponent; 

UCLASS()
class AKLConstructionActor : public AActor, public IKLInteractables
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKLConstructionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * Root Mesh Component
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp; 

	/**
	 * Box when overlapped, actors begin to interact in the building process. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* OverlapBox;

	/**
	 * The class of camera that the building actors attach to when interacted with; 
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraComponent> InteractorCamera;

	/**
	 * Is this actor being held by a player. 
	 */
	bool bIsGrabbed; 

	/**
	 * Defined in the child classes to find acceptable locations to place each type of building actor. 
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bCanPlace; 

	/**
	 * When two building actors are interacting, this is the socket on the actor -- that is currently being used by the player -- that is being chosen to attach to the other building actor 
	 */
	FName LocalSocketName;
	
	/**
	 * The transform of the actor that is not being used by the player
	 */
	FTransform SecondSocketTransform;

	/**
	 * When two building actors are interacting, one socket from each actor is chosen to interact (the closest 2 sockets). When these sockets change, this function is called to snap the actor 
	 * into a new location
	 */
	void SnapToCompatableComponent(FName Check1, FTransform Check2); 

	UFUNCTION() 
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 

	/**
	 * Called from Tick() to find the nearest two sockets on the two interacting actors. 
	 */
	void FindAttachements(); 

	/**
	 * When this actor is being used by the player, and this actor begins overlap with another building actor, this String key is the name of the sockets on the other actor that are compatible
	 * with this building type. 
	 */
	UPROPERTY(EditDefaultsOnly)
	FString SocketTypeToMatch; 


	TArray<FName> AttachSockets;

	TArray<FName> RelevantSockets; 

	UPROPERTY(EditDefaultsOnly)
	FString AttachSocketKey;

	UPROPERTY()
	AKLConstructionActor* CompatableActor; 

	bool bIsPlaced; 

	bool bSnappedToActor; 

	virtual void AdjustRotation(FTransform TransformAdjustments, FRotator CurrentSocketRotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Focused(APawn* Interactor) override;

	virtual void InteractedWith() override; 

	virtual void EndFocus() override; 

	TArray<FTransform> GetAttachPointSockets(FString AttachSocketType);

	bool IsPlaced();

	FString GetSocketTypeToMatch(); 

private: 
	bool bIsOverlappingWithCompatableActor; 
};
