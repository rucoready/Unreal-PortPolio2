// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Interface.h"
#include "../MainCharacter/HitInterface.h"
#include "../AI/HitTestCrow.h"
#include "../MainCharacter/SwordCharacter.h"
#include "EngineUtils.h"
#include "../GameManager/StatGameInstance.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "../Task/AIStateEnum.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASwordActor::ASwordActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);

	swordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordMesh"));
	swordMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> crowMesh(TEXT("/Game/Luco/Sword/SM_BasicSword.SM_BasicSword"));
	if (crowMesh.Succeeded())
	{
		swordMesh->SetStaticMesh(crowMesh.Object);
	}
	swordCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsuleComp"));
	swordCapsuleComp->SetupAttachment(RootComponent);

	swordCapsuleComp->SetRelativeLocation(FVector(-54.0f, 0.0f, 0.0f));
	swordCapsuleComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	swordCapsuleComp->SetWorldScale3D(FVector(1.0, 0.53, 1.17));

	capsureTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("startPoint"));
	capsureTraceStartPoint->SetupAttachment(RootComponent);
	capsureTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	capsureTraceEndPoint->SetRelativeLocation(FVector(-102, 0, 0));
	capsureTraceEndPoint->SetupAttachment(RootComponent);

	

	
}

// Called when the game starts or when spawned
void ASwordActor::BeginPlay()
{
	Super::BeginPlay();
	//overlap
	swordCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordActor::OnBeginOverlapSwordCollision);
	bSwordAttackCollisionActivate = false;

	swordCharacter = Cast<ASwordCharacter>(GetAttachParentActor());
	//swordCharacter->getDamageDatableValue(skillDamage);
	
	statGI = Cast<UStatGameInstance>(GetGameInstance());
	//getDamageDatableValue();
}

// Called every frame
void ASwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSwordAttackCollisionActivate)
	{
		return;
	}
	const FVector start = capsureTraceStartPoint->GetComponentLocation();
	const FVector end = capsureTraceEndPoint->GetComponentLocation();
	//ignoreSelf
	TArray<AActor*> self;
	self.Add(this);

	FHitResult capsuleHit;
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(this, start, end,
		FVector(5.f, 5.f, 5.f),
		capsureTraceStartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		self,
		EDrawDebugTrace::None,
		//EDrawDebugTrace::ForDuration,
		capsuleHit,
		true
	);

	
	
	
}



void ASwordActor::OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bSwordAttackCollisionActivate || !OtherActor || ignoreActors.Contains(OtherActor))
	{
		return;
	}

	if (IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor))
	{
		FVector hitLocation = SweepResult.ImpactPoint;
		float damage = 0.f;
		if (swordCharacter->skillName == "Q")
		{
			damage = statGI->qAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "QF")
		{
			damage = statGI->qAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "W")
		{
			damage = statGI->wAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "WF")
		{
			damage = statGI->wAttackDamage + FMath::RandRange(5, 20) + 50;
		}
		else if (swordCharacter->skillName == "E")
		{
			damage = statGI->eAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "EF")
		{
			damage = statGI->eAttackDamage + FMath::RandRange(5, 20) + 50;
		}
		else if (swordCharacter->skillName == "R")
		{
			damage = statGI->rAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "RF")
		{
			damage = statGI->rAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "Normal")
		{
			damage = statGI->normalAttackDamage + FMath::RandRange(5, 20);
		}
		else if (swordCharacter->skillName == "NormalF")
		{
			damage = statGI->normalAttackDamage + FMath::RandRange(5, 20) + 30;
		}

		
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
		
		

		//blackboard
		APawn* myPawn = Cast<APawn>(OtherActor);
		bool bIsSpawned;
		
		if (myPawn)
		{
			AAIController* skCon = Cast<AAIController>(myPawn->GetController());
			if (skCon)
			{
				UBlackboardComponent* blackBoard = skCon->GetBlackboardComponent();
				if (blackBoard)
				{
					bIsSpawned = blackBoard->GetValueAsBool(TEXT("Spawn"));
				}
			}
		}
		if (OtherActor->GetName().Contains("Crow"))
		{
			crow = Cast<AHitTestCrow>(OtherActor);
			if(crow)
			{
				crow->ShowHud(calculateDamage);
			}
		}
		else if (OtherActor->GetName().Contains("Skeleton") && bIsSpawned)
		{
			skeleton = Cast<ABasicSkeletonEnemy>(OtherActor);
			if (skeleton && skeleton->currentHp > 0.0f)
			{
				skeleton->ShowHud(calculateDamage);
			}
		}
		hitInterface->GetHit(hitLocation);
	}
	ignoreActors.AddUnique(OtherActor);
}

void ASwordActor::SetEnableAttackCollision()
{
	swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ignoreActors.Empty();
	bSwordAttackCollisionActivate = true;
	

}

void ASwordActor::SetDisableAttackCollision()
{
	swordCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bSwordAttackCollisionActivate = false;
}

void ASwordActor::emptyIgnoreActorArr()
{
	ignoreActors.Empty();
}

void ASwordActor::getMyCharacterSkillType(FString skill, float damage)
{
	skillName = skill;
	skillDamage = damage;
	
}








void ASwordActor::ClearSkillName()
{
	skillName = "";
}

void ASwordActor::getDamageDatableValue()
{
	if (statGI)
	{
		normalAttackDamage = statGI->normalAttackDamage;
		normalLastAttackDamage = statGI->normalLastAttackDamage;
		qAttackDamage = statGI->qAttackDamage;
		qAttackLastDamage = statGI->qAttackLastDamage;
		wAttackDamage = statGI->wAttackDamage;
		wAttackLastDamage = statGI->wAttackLastDamage;
		eAttackDamage = statGI->eAttackDamage;
		eAttackLastDamage = statGI->eAttackLastDamage;
		rAttackDamage = statGI->rAttackDamage;
		rAttackLastDamage = statGI->rAttackLastDamage;
	}
}





