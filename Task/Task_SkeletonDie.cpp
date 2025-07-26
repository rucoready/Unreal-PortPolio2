// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonDie.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../GameManager/StatGameInstance.h"
#include "EngineUtils.h"
#include "Components/SkeletalMeshComponent.h"

UTask_SkeletonDie::UTask_SkeletonDie()
{
	NodeName = "Skeleton Die";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_SkeletonDie::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);
	minDissolveValue = -0.4f;
	maxDissolveValue = 0.7f;
	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
	gameMode->increaseSkeletonDieCount();
	//instance
	if (UWorld* world = ownerComp.GetWorld())
	{
		myInstance = Cast<UStatGameInstance>(world->GetGameInstance());
		myInstance->setDungeonSkeletonDieCountting();
	}
	FTaskSkeletonDieMemory* taskMemory = reinterpret_cast<FTaskSkeletonDieMemory*>(nodeMemory);
	taskMemory->structCurrentTime;
	ASkeletonAIController* skeletalController = Cast<ASkeletonAIController>(ownerComp.GetAIOwner());
	skeletalController->StopMovement();
	ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
	UGameplayStatics::PlaySoundAtLocation(this, dieSound, skeleton->GetActorLocation());
	skeleton->GetMesh()->SetSimulatePhysics(true);
	skeleton->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	skeleton->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	blackBoard->SetValueAsBool("Die", true);
	skeleton->sword->SetVisibility(false);

	//spawnItem
	FHitResult traceHitResult;
	FCollisionQueryParams traceParams;
	traceParams.bReturnPhysicalMaterial = false;
	traceParams.AddIgnoredActor(skeleton);
	for (TActorIterator<APawn>it(GetWorld()); it; ++it)
	{
		traceParams.AddIgnoredActor(*it);
	}
	FVector traceStart = skeleton->GetActorLocation();
	FVector traceEnd = traceStart - FVector(0.f, 0.f, 10000.f);
	bool bHitTrace = GetWorld()->LineTraceSingleByChannel(traceHitResult, traceStart, traceEnd, ECC_WorldStatic, traceParams);
	
	if (bHitTrace)
	{
		float randomOffset = FMath::FRandRange(0.f, 30.f);
		bool spawnbOffSetx = FMath::RandBool();
		FVector spawnOffset = FVector(0.f, 0.f, 0.f);
		if (spawnbOffSetx)
		{
			spawnOffset.X = randomOffset;
		}
		else
		{
			spawnOffset.Y = randomOffset;
		}
		FVector itemSpawnLoc = traceHitResult.ImpactPoint + FVector(0.f, 0.f, 10.f) + spawnOffset;
		FRotator itemSpawnRot = skeleton->GetActorRotation();
		if (moneyActor)
		{
			GetWorld()->SpawnActor<AActor>(moneyActor, itemSpawnLoc, itemSpawnRot);
		}
		if (soulActor)
		{
			GetWorld()->SpawnActor<AActor>(soulActor, itemSpawnLoc, itemSpawnRot);
		}

		if (gearActor && FMath::FRand() <= 0.25f)
		{
			GetWorld()->SpawnActor<AActor>(gearActor, itemSpawnLoc, itemSpawnRot);
		}
		if (potionActorHp && FMath::FRand() <= 0.5f)
		{
			int32 potionProp = FMath::RandRange(0, 4);
			{
				if (potionProp == 0)
				{
					GetWorld()->SpawnActor<AActor>(potionActorHp, itemSpawnLoc, itemSpawnRot);
				}
				if (potionProp == 1)
				{
					GetWorld()->SpawnActor<AActor>(potionActorMp, itemSpawnLoc, itemSpawnRot);
				}
				if (potionProp == 2)
				{
					GetWorld()->SpawnActor<AActor>(potionActorForce, itemSpawnLoc, itemSpawnRot);
				}
				if (potionProp == 3)
				{
					GetWorld()->SpawnActor<AActor>(potionActorElic, itemSpawnLoc, itemSpawnRot);
				}
				else
				{
					GetWorld()->SpawnActor<AActor>(potionActorP, itemSpawnLoc, itemSpawnRot);
				}
			}

		}
	}
	
	
	return EBTNodeResult::InProgress;
}

void UTask_SkeletonDie::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	FTaskSkeletonDieMemory* taskMemory = reinterpret_cast<FTaskSkeletonDieMemory*>(nodeMemory);

	taskMemory->structCurrentTime += deltaSeconds;
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	
	if (taskMemory->structCurrentTime >= 7.0f)
	{
		//만약 깡으로 계산한다면
		//float dissolveValue = FMath::Lerp(minDissolveValue, maxDissolveValue, taskMemory->structCurrentTime / 9.0f);

		float dissolveAlpha = FMath::Clamp((taskMemory->structCurrentTime - 7.0f) / 2.0f, 0.0f, 1.0f);
		float dissolveValue = FMath::Lerp(minDissolveValue, maxDissolveValue, dissolveAlpha);
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		int32 materialCount = skeleton->GetMesh()->GetNumMaterials();
		for (int32 i = 0; i < materialCount; ++i)
		{
			int32 materialIndex = i;
			UMaterialInstanceDynamic* dynMat = skeleton->GetMesh()->CreateAndSetMaterialInstanceDynamic(materialIndex);
			if (dynMat)
			{
				dynMat->SetScalarParameterValue("Dissolve", dissolveValue);
			}
		}
		if (skeleton->sword)
		{
			int32 swordMatCount = skeleton->sword->GetNumMaterials();
			for (int32 i = 0; i < swordMatCount; ++i)
			{
				UMaterialInstanceDynamic* swordMat = skeleton->sword->CreateAndSetMaterialInstanceDynamic(i);
				if (swordMat)
				{
					swordMat->SetScalarParameterValue("Dissolve", dissolveValue);
				}
			}
		}
		
	}
	if (taskMemory->structCurrentTime >= 9.1f)
	{
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		skeleton->Destroy();
	}
}
