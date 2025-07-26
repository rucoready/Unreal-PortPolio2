// Fill out your copyright notice in the Description page of Project Settings.


#include "BowSkeletonProjectTile.h"
#include "Components/CapsuleComponent.h"
#include "../MainCharacter/SwordCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABowSkeletonProjectTile::ABowSkeletonProjectTile()
{
	PrimaryActorTick.bCanEverTick = true;
	//Scene
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	RootComponent = sceneComp;
	//Collision
	collisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComponent"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetupAttachment(RootComponent);
	//SkeletalMeshComp
	projectTileComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("arrowMesh"));
	projectTileComp->SetupAttachment(RootComponent);
	projectTileComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ProjectTile
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectTileMevement"));
	projectileMovement->InitialSpeed = 2500.f;
	projectileMovement->MaxSpeed = 2500.f;
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->ProjectileGravityScale = 0.3f; 
}

void ABowSkeletonProjectTile::BeginPlay()
{
	Super::BeginPlay();
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABowSkeletonProjectTile::OnBeginOverlap);
}

void ABowSkeletonProjectTile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString hitActorName = OtherActor->GetName();
	if (hitActorName.Contains(TEXT("SwordCharacter")))
	{
		swordPlayer = Cast<ASwordCharacter>(OtherActor);
		swordPlayer->GetDamage(projectTileDamage);
		Destroy();
	}
}


void ABowSkeletonProjectTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

