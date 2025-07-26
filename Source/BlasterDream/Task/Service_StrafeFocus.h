// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AIStateEnum.h"
#include "Service_StrafeFocus.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UService_StrafeFocus : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:	
	UService_StrafeFocus();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EAIStateEnum currentState;
};
