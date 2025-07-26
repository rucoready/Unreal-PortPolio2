// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_SkeletonSpeedChange.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UTask_SkeletonSpeedChange : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_SkeletonSpeedChange();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float aiSpeed;
};
