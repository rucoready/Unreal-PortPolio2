// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonSpeedChange.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UTask_SkeletonSpeedChange::UTask_SkeletonSpeedChange()
{
	NodeName = "Change Speed";
}

EBTNodeResult::Type UTask_SkeletonSpeedChange::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);
	ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
	skeleton->GetCharacterMovement()->MaxWalkSpeed = aiSpeed;
	return EBTNodeResult::Succeeded;
}
