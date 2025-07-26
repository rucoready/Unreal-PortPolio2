// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonBeDamaged.h"
#include "../AI/SkeletonAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIStateEnum.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_SkeletonBeDamaged::UTask_SkeletonBeDamaged()
{
	NodeName = TEXT("BeDamaged");
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_SkeletonBeDamaged::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);
	ASkeletonAIController* skeletonController = Cast<ASkeletonAIController>(ownerComp.GetAIOwner());
	skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
	FTaskSkeletonBeDamageMemory* taskMemory = reinterpret_cast<FTaskSkeletonBeDamageMemory*>(nodeMemory);
	taskMemory->structCurrentTime = 0.0f;
	return EBTNodeResult::InProgress;
	
}

void UTask_SkeletonBeDamaged::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	currentTime += deltaSeconds;
	FTaskSkeletonBeDamageMemory* taskMemory = reinterpret_cast<FTaskSkeletonBeDamageMemory*>(nodeMemory);
	taskMemory->structCurrentTime += deltaSeconds;
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	if (skeleton->bDamaged)
	{
		currentTime = 0.0f;  
		onceAttackFlag = false;
	
		
	}
	if (skeleton->currentHp <= 0.0f)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
	if (skeleton->pendingDamageCount > 0 && !onceAttackFlag)
	{
		skeleton->MotionWarpingPlayer();
		skeleton->DamagedMontagePlay();
		onceAttackFlag = true;
		skeleton->pendingDamageCount--; 
		if (skeleton->pendingDamageCount == 0)
			skeleton->bDamaged = false;
	}
	if (taskMemory->structCurrentTime > 0.9f)
	{
		onceAttackFlag = false;
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}


