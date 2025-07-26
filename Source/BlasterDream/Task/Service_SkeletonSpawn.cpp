// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_SkeletonSpawn.h"
#include "BehaviorTree/BlackboardComponent.h" 



UService_SkeletonSpawn::UService_SkeletonSpawn()
{
	NodeName = "Skeleton Spawn Check";
}

void UService_SkeletonSpawn::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	blackBoard->SetValueAsBool("Spawn", false);
}
