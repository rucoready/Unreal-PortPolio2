// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_CheckingDamaged.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "AIStateEnum.h"
#include "BehaviorTree/BlackboardComponent.h" 

UService_CheckingDamaged::UService_CheckingDamaged()
{
	NodeName = "SkeletonDamagedCheck";
	bNotifyTick = true;
}

void UService_CheckingDamaged::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (skeleton->bDamaged)
	{
		currentState = EAIStateEnum::Damaged;
		blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
		return;

	}
	if (skeleton->currentHp <= 0.0)
	{
		blackBoard->SetValueAsBool("Die", true);
		return;
	}
}
