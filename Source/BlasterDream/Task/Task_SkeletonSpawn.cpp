// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonSpawn.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h" 

UTask_SkeletonSpawn::UTask_SkeletonSpawn()
{
	NodeName = "Dissolve Material Activate";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_SkeletonSpawn::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::ExecuteTask(ownerComp, nodeMemory);
	minDissolveValue = -0.4f;
	maxDissolveValue = 0.7f;

	FTaskSkeletonSpawnMemory* taskMemory = reinterpret_cast<FTaskSkeletonSpawnMemory*>(nodeMemory);
	taskMemory->structCurrentTime = 0.0f;

	return EBTNodeResult::InProgress;
}

void UTask_SkeletonSpawn::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	FTaskSkeletonSpawnMemory* taskMemory = reinterpret_cast<FTaskSkeletonSpawnMemory*>(nodeMemory);

	taskMemory->structCurrentTime += deltaSeconds;
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	if (taskMemory->structCurrentTime < 3.0f)
	{
		
		float dissolveValue = FMath::Lerp(maxDissolveValue, minDissolveValue, taskMemory->structCurrentTime / 3.0f);
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		int32 materialCount = skeleton->GetMesh()->GetNumMaterials();
		for (int32 i = 0; i < materialCount; ++i)
		{
			int32 materialIndex = i;

			UMaterialInstanceDynamic* dynMat = skeleton->GetMesh()->CreateAndSetMaterialInstanceDynamic(materialIndex);
			if (dynMat)
			{
				dynMat->SetScalarParameterValue("Dissolve", dissolveValue);
			}
		}
		if (skeleton->sword)
		{
			int32 swordMatCount = skeleton->sword->GetNumMaterials();
			for (int32 i = 0; i < swordMatCount; ++i)
			{
				UMaterialInstanceDynamic* swordMat = skeleton->sword->CreateAndSetMaterialInstanceDynamic(i);
				if (swordMat)
				{
					swordMat->SetScalarParameterValue("Dissolve", dissolveValue);
				}
			}
		}
	}
	else
	{
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		blackBoard->SetValueAsBool("Spawn", true);
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}
















