// Fill out your copyright notice in the Description page of Project Settings.


#include "DownwardknockbackActor.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager/StatGameInstance.h"
#include "../AI/HitTestCrow.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MainCharacter/DragonCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ADownwardknockbackActor::ADownwardknockbackActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BoxCollision"));
	collisionSphere->SetupAttachment(RootComponent);
	collisionSphere->SetSphereRadius(50.0f);
	collisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	collisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}



void ADownwardknockbackActor::BeginPlay()
{
	Super::BeginPlay();
	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADownwardknockbackActor::OnBeginOverlapSphereCollision);
	statGI = Cast<UStatGameInstance>(GetGameInstance());

}

// Called every frame
void ADownwardknockbackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADownwardknockbackActor::OnBeginOverlapSphereCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //blackboard
    ASwordCharacter* castedSwordCharacter = Cast<ASwordCharacter>(OtherActor);
    APawn* myPawn = Cast<APawn>(OtherActor);
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
    if (OtherActor && OtherActor != this)
    {
        if (!ignoreActor.Contains(OtherActor))
        {
            FString hitActorName = OtherActor->GetName();

            if (hitActorName.Contains(TEXT("Crow")))
            {
                FVector sphereLoc = collisionSphere->GetComponentLocation();   
                FVector targetLoc = OtherActor->GetActorLocation();
                FVector direction = (targetLoc - sphereLoc).GetSafeNormal();

                ACharacter* enemyCharacter = Cast<ACharacter>(OtherActor);
                if (enemyCharacter)
                {
                    launchForce = 1500.f;
                    enemyCharacter->LaunchCharacter(direction * launchForce, true, true);
                    crow = Cast<AHitTestCrow>(OtherActor);
                    if (crow)
                    {
                        float damage = aBaseLastDamage + FMath::RandRange(20, 50) + 700;
                        
                        //crits
                        if (ASwordCharacter* swordCharacter = Cast<ASwordCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
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
                        }
                        crow->ShowHud(calculateDamage);
                    }
                }
            }

            else if (hitActorName.Contains(TEXT("Skeleton")) && bIsSpawned && !bIsDead)
            {
                FVector sphereLoc = collisionSphere->GetComponentLocation();
                FVector targetLoc = OtherActor->GetActorLocation();
                FVector direction = (targetLoc - sphereLoc).GetSafeNormal();

                ACharacter* enemyCharacter = Cast<ACharacter>(OtherActor);
                if (enemyCharacter)
                {
                    enemyCharacter->LaunchCharacter(direction * launchForce, true, true);
                    skeleton = Cast<ABasicSkeletonEnemy>(OtherActor);
                    if (skeleton)
                    {
                        float damage = aBaseLastDamage + FMath::RandRange(20, 50) + 700;
                        //crits
                        if (ASwordCharacter* swordCharacter = Cast<ASwordCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
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
                        }
                        else
                        {
                            int randomOrbDamage = FMath::FRandRange(3000.f, 3500.f);
                            calculateDamage = randomOrbDamage;
                        }
                       
                        skeleton->ShowHud(calculateDamage);
                        
                    }
                }
            }

            // 이미 처리된 Actor는 ignoreActor 리스트에 추가
            ignoreActor.AddUnique(OtherActor);
        }
    }
}
void ADownwardknockbackActor::getDamageDatableValue()
{
	//임시값
	if (statGI)
	{
		aBaseDamage = statGI->eAttackLastDamage;
		aBaseLastDamage = statGI->eAttackLastDamage;
	}
}