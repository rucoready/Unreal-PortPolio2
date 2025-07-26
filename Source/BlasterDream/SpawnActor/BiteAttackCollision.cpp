// Fill out your copyright notice in the Description page of Project Settings.


#include "BiteAttackCollision.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../AI/HitTestCrow.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MainCharacter/DragonCharacter.h"

// Sets default values
ABiteAttackCollision::ABiteAttackCollision()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	collisionBox->SetupAttachment(RootComponent);
	collisionBox->SetBoxExtent(FVector(50.0f));
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	collisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABiteAttackCollision::BeginPlay()
{
	Super::BeginPlay();
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABiteAttackCollision::OnBeginOverlapBoxCollision);
	APawn* myCharacterPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABiteAttackCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABiteAttackCollision::OnBeginOverlapBoxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bIsSpawned;
	bool bIsDead;
	FString hitActorName = OtherActor->GetName();
	ABasicSkeletonEnemy* skeletonEnemy = Cast<ABasicSkeletonEnemy>(OtherActor);
	if (skeletonEnemy)
	{
		AAIController* AICon = Cast<AAIController>(skeletonEnemy->GetController());
		if (AICon)
		{
			UBlackboardComponent* blackBoard = AICon->GetBlackboardComponent();
			if (blackBoard)
			{
				bIsSpawned = blackBoard->GetValueAsBool(TEXT("Spawn"));
				bIsDead = blackBoard->GetValueAsBool(TEXT("Die"));
			}
		}
	}
	if (hitActorName.Contains(TEXT("Crow")))
	{
		crow = Cast<AHitTestCrow>(OtherActor);
		if (crow)
		{
			//crits
			int randomOrbDamage = FMath::FRandRange(2000.f, 2500.f);
			calculateDamage = randomOrbDamage;
			crow->ShowHud(calculateDamage);
			UGameplayStatics::SpawnSoundAtLocation(this, biteSound, GetActorLocation());
		}
	}
	else if (hitActorName.Contains(TEXT("Skeleton")) && bIsSpawned && !bIsDead)
	{
		skeleton = Cast<ABasicSkeletonEnemy>(OtherActor);
		if (skeleton)
		{
			int randomOrbDamage = FMath::FRandRange(2000.f, 2500.f);
			calculateDamage = randomOrbDamage;
			skeleton->ShowHud(calculateDamage);
			UGameplayStatics::SpawnSoundAtLocation(this, biteSound, GetActorLocation());
		}
	}
}

