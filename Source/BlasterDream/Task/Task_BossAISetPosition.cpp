// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_BossAISetPosition.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTask_BossAISetPosition::UTask_BossAISetPosition()
{
	NodeName = "PositionSet";
}

EBTNodeResult::Type UTask_BossAISetPosition::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
	AAIController* aiCon = ownerComp.GetAIOwner();
	APawn* aiPawn = aiCon->GetPawn();
	FVector randomOffset = FVector(FMath::FRandRange(-offSetRange, offSetRange),FMath::FRandRange(-offSetRange, offSetRange),
		0.f 
	);
	FVector finalPosition = positionVector + randomOffset;
	blackboardComp->SetValueAsVector(positionVectorKey.SelectedKeyName, finalPosition);
	return EBTNodeResult::Succeeded;
}

