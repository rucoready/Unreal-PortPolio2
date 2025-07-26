// Fill out your copyright notice in the Description page of Project Settings.


#include "AOEAttackCollisionActor.h"
#include "Components/BoxComponent.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../AI/HitTestCrow.h"
#include "../MainCharacter/HitInterface.h"
#include "../MainCharacter/SwordAnimInstance.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../GameManager/StatGameInstance.h"

// Sets default values
AAOEAttackCollisionActor::AAOEAttackCollisionActor()
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


void AAOEAttackCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAOEAttackCollisionActor::OnBeginOverlapBoxCollision);

	statGI = Cast<UStatGameInstance>(GetGameInstance());
	getDamageDatableValue();
	APawn* myCharacterPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	swordCharacter = Cast<ASwordCharacter>(myCharacterPawn);

	if (swordCharacter && swordCharacter->GetMesh())
	{
		animInstance = swordCharacter->GetMesh()->GetAnimInstance();
		animIT = Cast<USwordAnimInstance>(animInstance);


	}

}

// Called every frame
void AAOEAttackCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAOEAttackCollisionActor::OnBeginOverlapBoxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || ignoreActor.Contains(OtherActor))
    {
        return;
    }
    //blackboard
    APawn* myPawn = Cast<APawn>(OtherActor);
    ASwordCharacter* castedSwordCharacter = Cast<ASwordCharacter>(OtherActor);
    bool bIsSpawned;
    bool bIsDead;
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
    
    if (IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor))
    {
        FString hitActorName = OtherActor->GetName();
        FVector hitLocation = SweepResult.ImpactPoint;

        float damage = 0.f;
        if (swordCharacter->skillName == "QF")
        {
            damage = statGI->qAttackLastDamage + FMath::RandRange(5, 20);
        }
        else if (swordCharacter->skillName == "WF")
        {
            damage = statGI->wAttackLastDamage + FMath::RandRange(5, 20);
        }
        else if (swordCharacter->skillName == "EF")
        {
            if (animIT->forceN == 4)
            {
                damage = statGI->eAttackLastDamage + FMath::RandRange(5, 20) + 350;
            }
            else
            {
                damage = statGI->eAttackDamage + FMath::RandRange(5, 20);
            }
        }
        else if (swordCharacter->skillName == "E")
        {
            if (animIT->forceN == 3)
            {
                damage = statGI->eAttackLastDamage + FMath::RandRange(5, 20);
            }
            else
            {
                damage = statGI->eAttackDamage + FMath::RandRange(5, 20);
            }
        }
        else if (swordCharacter->skillName == "RF")
        {
            if (animIT->forceN == 0)
            {
                damage = statGI->rAttackDamage + FMath::RandRange(5, 20) + 50;
            }
            else if (animIT->forceN == 1)
            {
                damage = statGI->rAttackLastDamage + FMath::RandRange(5, 20) + 50;
            }
        }
        else if (swordCharacter->skillName == "R")
        {
            if (animIT->forceN == 0)
            {
                damage = statGI->rAttackDamage + FMath::RandRange(5, 20);
            }
            else if (animIT->forceN == 1)
            {
                damage = statGI->rAttackLastDamage + FMath::RandRange(5, 20);
            }
        }
        else if (swordCharacter->skillName == "S")
        {
            damage = statGI->rAttackLastDamage + FMath::RandRange(10, 40) + 500;
        }
        if (hitActorName.Contains(TEXT("Crow")))
        {
            crow = Cast<AHitTestCrow>(OtherActor);
            if (crow)
            {
                //crits
                int32 entireCritChance = FMath::RandRange(1, 100);
                if (entireCritChance <= swordCharacter->critsPercent)
                {
                    //crits X%
                    calculateDamage = damage * 1.5;
                    swordCharacter->bCritical = true;
                }
                else
                {
                    calculateDamage = damage;
                    swordCharacter->bCritical = false;
                }
                crow->ShowHud(calculateDamage);
                
            }
        }
        else if (hitActorName.Contains(TEXT("Skeleton")) && bIsSpawned)
        {
            skeleton = Cast<ABasicSkeletonEnemy>(OtherActor);
            if (skeleton && skeleton->currentHp > 0.0f)
            {
                //crits
                int32 entireCritChance = FMath::RandRange(1, 100);
               
               if (entireCritChance <= swordCharacter->critsPercent)
               {
                    //crits X%
                    calculateDamage = damage * 1.5;
                    swordCharacter->bCritical = true;
                }
                else
                {
                    calculateDamage = damage;
                    swordCharacter->bCritical = false;
                }
               skeleton->ShowHud(calculateDamage);
                
                
                
            }
        }

        hitInterface->GetHit(hitLocation);

        ignoreActor.AddUnique(OtherActor);
    }
}

void AAOEAttackCollisionActor::getDamageDatableValue()
{
	if (statGI)
	{
		normalLastAttackDamage = statGI->normalLastAttackDamage;
		qAttackLastDamage = statGI->qAttackLastDamage;
		wAttackLastDamage = statGI->wAttackLastDamage;
		wAttackDamage = statGI->wAttackDamage;
		eAttackDamage = statGI->eAttackDamage;
		eAttackLastDamage = statGI->eAttackLastDamage;
		rAttackLastDamage = statGI->rAttackLastDamage;
		rAttackDamage = statGI->rAttackDamage;
	}
}

