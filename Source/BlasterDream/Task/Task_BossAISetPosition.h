// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossAISetPosition.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UTask_BossAISetPosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UTask_BossAISetPosition();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	FVector positionVector;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float offSetRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FBlackboardKeySelector positionVectorKey;
};
