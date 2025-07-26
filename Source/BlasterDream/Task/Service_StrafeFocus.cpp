// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_StrafeFocus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_StrafeFocus::UService_StrafeFocus()
{
	NodeName = "Strafe Focus";
	bNotifyTick = true;
}

void UService_StrafeFocus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* aICon = OwnerComp.GetAIOwner();
	if (aICon == nullptr)
	{
		return;
	}
	UBlackboardComponent* blackBoard = aICon->GetBlackboardComponent();
	UObject* targetObject = blackBoard->GetValueAsObject(GetSelectedBlackboardKey());

	currentState = (EAIStateEnum)blackBoard->GetValueAsEnum(TEXT("AIState"));
	AActor* targetActor = Cast<AActor>(blackBoard->GetValueAsObject("Target"));
	if (targetActor)
	{
		FVector TargetLocation = targetActor->GetActorLocation();
		aICon->SetFocalPoint(TargetLocation);
	}
	
}
