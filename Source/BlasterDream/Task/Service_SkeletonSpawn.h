// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Service_SkeletonSpawn.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UService_SkeletonSpawn : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UService_SkeletonSpawn();
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
