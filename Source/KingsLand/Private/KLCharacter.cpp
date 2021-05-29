// Fill out your copyright notice in the Description page of Project Settings.


#include "KLCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "KLWeapon.h"
#include "KingsLand/KingsLand.h"
#include "KLBow.h"

// Sets default values
AKLCharacter::AKLCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom"); 
	CameraBoom->SetupAttachment(GetCapsuleComponent()); 

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera"); 
	Camera->SetupAttachment(CameraBoom); 

	
	RotationSpeed = 100.f; 
	MaxLookUpAngle = 50.f; 

	MovementInput = false; 

	WeaponSocketName = "WeaponSocket";

	bIsAiming = false; 

	AimWalkSpeed = 150.f; 
	BaseWalkSpeed = 600.f; 

	BoomAttachSocket = "BoomSocket"; 
}

// Called when the game starts or when spawned
void AKLCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();  
	if(World != nullptr)
	{
		FActorSpawnParameters Params; 
		Params.Owner = this; 
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 
		const FVector WeaponSpawnLocation = GetMesh()->GetSocketLocation(WeaponSocketName);
		const FRotator WeaponsSpawnRotation = GetMesh()->GetSocketRotation(WeaponSocketName); 
		AActor* Weapon = World->SpawnActor(PrimaryWeaponClass, &WeaponSpawnLocation, &WeaponsSpawnRotation, Params);
		if(Weapon != nullptr)
		{
			EquipedWeapon = Cast<AKLWeapon>(Weapon);
			if (EquipedWeapon != nullptr)
			{
				FAttachmentTransformRules TransRule = FAttachmentTransformRules::KeepWorldTransform; 
				EquipedWeapon->AttachToComponent(GetMesh(), TransRule, WeaponSocketName);
			}
		}
	}
}

// Called every frame
void AKLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKLCharacter::MoveForward); 
	PlayerInputComponent->BindAxis("MoveRight", this, &AKLCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput); 
	PlayerInputComponent->BindAxis("LookUp", this, &AKLCharacter::LookUp);

	PlayerInputComponent->BindAction("UseWeapon", IE_Pressed, this, &AKLCharacter::UseWeapon);
	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this, &AKLCharacter::ReleaseBow);
}


void AKLCharacter::MoveForward(float Val)
{
	AddMovementInput(Val * GetActorForwardVector()); 
}

void AKLCharacter::MoveRight(float Val)
{
	AddMovementInput(Val * GetActorRightVector());
}

void AKLCharacter::LookRight(float Val)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), Val);
	FRotator CapsuleRotation = GetCapsuleComponent()->GetComponentRotation();
	CapsuleRotation.Yaw += Val * RotationSpeed * ObDeltaTime;
	NewRotation.Yaw = CapsuleRotation.Yaw; 
}

void AKLCharacter::LookUp(float Val)
{
	FRotator CameraBoomRotation = CameraBoom->GetComponentRotation();
	CameraBoomRotation.Pitch += Val; 
	CameraBoomRotation.Pitch = FMath::Clamp(CameraBoomRotation.Pitch, -MaxLookUpAngle, MaxLookUpAngle); 
	CameraBoom->SetWorldRotation(CameraBoomRotation); 

	LookUpRotation = CameraBoomRotation.Pitch; 
}

void AKLCharacter::UseWeapon()
{
	if(EquipedWeapon != nullptr)
	{
		if(EquipedWeapon->bIsBow)
		{
			
			bIsAiming = EquipedWeapon->StartUseWeapon();
			if (bIsAiming)
			{
				GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
			}
		}
	}
}

void AKLCharacter::ReleaseBow()
{
	UE_LOG(LogTemp, Warning, TEXT("ReleaseBow:"));

	AKLBow* EquippedBow = Cast<AKLBow>(EquipedWeapon); 
	if(EquippedBow != nullptr && bIsAiming)
	{

		UE_LOG(LogTemp, Warning, TEXT("Bow Exists"));
		FMinimalViewInfo CameraViewInfo; 
		Camera->GetCameraView(GetWorld()->DeltaTimeSeconds, CameraViewInfo); 
		FRotator CameraRotation = CameraViewInfo.Rotation; 
		EquippedBow->ReleaseBow(CameraRotation);

		bIsAiming = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed; 
	}
}

