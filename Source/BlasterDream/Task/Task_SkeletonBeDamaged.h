// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "../AI/SkeletonAIController.h"
#include "AIStateEnum.h"
#include "Task_SkeletonBeDamaged.generated.h"

/**
 * 
 */
USTRUCT()
struct FTaskSkeletonBeDamageMemory
{
	GENERATED_BODY()

	float structCurrentTime = 0.0f;
	float randomEndTime;
};

UCLASS()
class BLASTERDREAM_API UTask_SkeletonBeDamaged : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_SkeletonBeDamaged();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;

	ASkeletonAIController* aiController;
	class ABasicSkeletonEnemy* skeleton;
	float currentTime;
	bool onceAttackFlag;
	EAIStateEnum currentState;


};
