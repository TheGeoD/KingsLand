// Fill out your copyright notice in the Description page of Project Settings.


#include "KLProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "KingsLand/KingsLand.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKLProjectile::AKLProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Collision"); 
	RootComponent = CollisionSphere; 
	CollisionSphere->SetCollisionProfileName("Arrow"); 
	CollisionSphere->OnComponentHit.AddDynamic(this, &AKLProjectile::OnHit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp"); 
	Mesh->SetupAttachment(RootComponent); 
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore); 

	PrimaryActorTick.bCanEverTick = true;
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp"); 
	MovementComp->UpdatedComponent = RootComponent; 
	MovementComp->InitialSpeed = 0.f; 
	MovementComp->bRotationFollowsVelocity = true; 
	MovementComp->bShouldBounce = false; 

	Damage = 0.f; 

}

// Called when the game starts or when spawned
void AKLProjectile::BeginPlay()
{
	Super::BeginPlay();	
}


void AKLProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile"));
}

void AKLProjectile::InitializeRelease(float Speed, AController* Controller, TArray<AActor*> IgnoringActors)
{
	MovementComp->InitialSpeed = Speed; 
	OwningController = Controller; 
	
	for(AActor* ToIgnore : IgnoringActors)
	{
		CollisionSphere->IgnoreActorWhenMoving(ToIgnore, true); 
	}
}
