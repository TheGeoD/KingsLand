// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KLCharacter.generated.h"

class UCameraComponent; 
class USpringArmComponent; 
class USkeletalMeshComponent; 
class AKLWeapon; 

UCLASS(BlueprintType)
class AKLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* CameraBoom; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKLWeapon> PrimaryWeaponClass; 

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera; 

	UPROPERTY()
	AKLWeapon* EquipedWeapon; 

	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;

	FVector DeltaLocation; 

	FRotator NewRotation; 

	float ObDeltaTime;

	UFUNCTION()
	void MoveForward(float Val); 

	UFUNCTION()
	void MoveRight(float Val);

	bool MovementInput; 

	FVector Veloctiy; 

	void LookRight(float Val); 

	void LookUp(float Val); 

	UFUNCTION()
	void UseWeapon(); 

	UFUNCTION()
	void ReleaseBow();

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed; 

	UPROPERTY(EditDefaultsOnly)
	float MaxLookUpAngle;

	UPROPERTY()
	float DrawStartTime; 

	UPROPERTY(VisibleAnywhere)
	float LookUpRotation; 

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming;

	UPROPERTY(EditDefaultsOnly)
	float AimWalkSpeed; 

	float BaseWalkSpeed; 

	UPROPERTY(EditDefaultsOnly)
	FName BoomAttachSocket; 

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
