// Fill out your copyright notice in the Description page of Project Settings.


#include "KLArrow.h"
#include "Kismet/GameplayStatics.h"

AKLArrow::AKLArrow()
{

}

void AKLArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Arrow"));
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, Hit.Normal, Hit, OwningController, this, UDamageType::StaticClass());

	AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); 
}
