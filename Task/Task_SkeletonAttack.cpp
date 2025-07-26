// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonAttack.h"
#include "../AI/SkeletonAIController.h"
#include "AIStateEnum.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "../AI/BasicSkeletonEnemy.h"

UTask_SkeletonAttack::UTask_SkeletonAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_SkeletonAttack::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);;
	onceAttackFlag = false;
	
	FTaskSkeletonAttackMemory* taskMemory = reinterpret_cast<FTaskSkeletonAttackMemory*>(nodeMemory);
	taskMemory->structCurrentTime = 0.0f;

	return EBTNodeResult::InProgress;
	
}

void UTask_SkeletonAttack::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);

	FTaskSkeletonAttackMemory* taskMemory = reinterpret_cast<FTaskSkeletonAttackMemory*>(nodeMemory);
	taskMemory->structCurrentTime += deltaSeconds;

	ASkeletonAIController* skeletonController = Cast<ASkeletonAIController>(ownerComp.GetAIOwner());
	
	
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
	if (skeleton->bDamaged)
	{
		currentState = EAIStateEnum::Damaged;
		blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);

	}
	if (skeleton->currentHp <= 0.0f)
	{
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
	if (!onceAttackFlag)
	{
		if (taskMemory->structCurrentTime > 0.0f)
		{
			skeleton->PlayAttackAnimation();
			onceAttackFlag = true;
			taskMemory->randomEndTime = FMath::FRandRange(2.0f, 3.5f);
		}
	}
	if (taskMemory->structCurrentTime > taskMemory->randomEndTime)
	{
		onceAttackFlag = false;
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}
