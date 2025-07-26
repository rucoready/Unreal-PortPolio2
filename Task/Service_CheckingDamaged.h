// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AIStateEnum.h"
#include "Service_CheckingDamaged.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UService_CheckingDamaged : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UService_CheckingDamaged();
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EAIStateEnum currentState;
};
