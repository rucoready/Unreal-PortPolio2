// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_bForcingSpawnTrue.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UTask_bForcingSpawnTrue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UTask_bForcingSpawnTrue();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

};
