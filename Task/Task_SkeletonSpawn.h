// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_SkeletonSpawn.generated.h"

/**
 * 
 */
USTRUCT()
struct FTaskSkeletonSpawnMemory
{
	GENERATED_BODY()

	float structCurrentTime = 0.0f;
};

UCLASS()
class BLASTERDREAM_API UTask_SkeletonSpawn : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UTask_SkeletonSpawn();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	float currentTime = 0.0f;
	float minDissolveValue;
	float maxDissolveValue;

	
};
