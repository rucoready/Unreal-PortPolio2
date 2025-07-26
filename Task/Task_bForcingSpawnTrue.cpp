// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_bForcingSpawnTrue.h"
#include "BehaviorTree/BlackboardComponent.h" 

UTask_bForcingSpawnTrue::UTask_bForcingSpawnTrue()
{
	NodeName = "ForcingSpawn";
}

EBTNodeResult::Type UTask_bForcingSpawnTrue::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	blackBoard->SetValueAsBool("Spawn", true);
	return EBTNodeResult::Succeeded; 
}
