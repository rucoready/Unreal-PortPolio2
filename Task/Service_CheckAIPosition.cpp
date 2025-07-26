// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_CheckAIPosition.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UService_CheckAIPosition::UService_CheckAIPosition()
{
	NodeName = "PositionCheck";
	bNotifyTick = true;
}

void UService_CheckAIPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* aICon = OwnerComp.GetAIOwner();
	APawn* aiPawn = aICon->GetPawn();
	UBlackboardComponent* blackBoard = aICon->GetBlackboardComponent();
	FVector lastPositionVector = blackBoard->GetValueAsVector(positionKey.SelectedKeyName);
	FVector currentAIPosition = aiPawn->GetActorLocation();

	FVector lastPos2D(lastPositionVector.X, lastPositionVector.Y, 0.f);
	FVector currentPos2D(currentAIPosition.X, currentAIPosition.Y, 0.f);

	//오차범위
	const float allowableArrivalRange = 400.f;
	const float allowableArrivalRangeSq = allowableArrivalRange * allowableArrivalRange;

	if (FVector::DistSquared(currentPos2D, lastPos2D) < allowableArrivalRangeSq)
	{
		blackBoard->SetValueAsBool(bAIPositionSetKey.SelectedKeyName, true);
	}
}
