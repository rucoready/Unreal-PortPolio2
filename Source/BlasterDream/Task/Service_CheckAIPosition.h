// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_CheckAIPosition.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UService_CheckAIPosition : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UService_CheckAIPosition();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FBlackboardKeySelector positionKey;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FBlackboardKeySelector bAIPositionSetKey;
};
