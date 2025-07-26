// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "../AI/SkeletonAIController.h"
#include "AIStateEnum.h"
#include "Task_SkeletonMoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UTask_SkeletonMoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_SkeletonMoveToPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;

	UBehaviorTreeComponent* cOwnerComp;
	EAIStateEnum currentState;
	
	UPROPERTY(EditAnywhere, Category="MySettings")
	USoundBase* chaseSound;
};
