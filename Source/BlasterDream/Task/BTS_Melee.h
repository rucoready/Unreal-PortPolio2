// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "AIStateEnum.h"
#include "../AI/SkeletonAIController.h"
#include "BTS_Melee.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UBTS_Melee : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_Melee();
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FVector playerLoc;
	//class ABasicSkeletonEnemy* skeleton;
	//class ASkeletonAIController* aiController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAIStateEnum currentState;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float attackDistance;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float dragonAttackDistance;

};
