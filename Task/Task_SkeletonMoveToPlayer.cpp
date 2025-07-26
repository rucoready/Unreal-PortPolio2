// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_SkeletonMoveToPlayer.h"
#include "../AI/SkeletonAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIStateEnum.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


UTask_SkeletonMoveToPlayer::UTask_SkeletonMoveToPlayer()
{
    NodeName = TEXT("Move To Target SwordPlayer");
    bNotifyTick = true;
}

EBTNodeResult::Type UTask_SkeletonMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    Super::ExecuteTask(ownerComp, nodeMemory);
    

    cOwnerComp = &ownerComp;
    float growlValue = FMath::FRand();
    if (growlValue < 0.3)
    {
        AAIController* aiController = ownerComp.GetAIOwner();
        if (aiController)
        {
            APawn* controlledPawn = aiController->GetPawn();
            if (controlledPawn)
            {
                FVector location = controlledPawn->GetActorLocation();
                UGameplayStatics::PlaySoundAtLocation(this, chaseSound, location);
            }
        }
    }


    return EBTNodeResult::InProgress;
}

void UTask_SkeletonMoveToPlayer::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
    Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
    UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
    cOwnerComp = &ownerComp;
    ASkeletonAIController* skeletalController = Cast<ASkeletonAIController>(ownerComp.GetAIOwner());
    UObject* targetObject = blackboardComp->GetValueAsObject(GetSelectedBlackboardKey());
    AActor* targetActor = Cast<AActor>(targetObject);
    currentState = (EAIStateEnum)blackboardComp->GetValueAsEnum(TEXT("AIState"));
    if (currentState != EAIStateEnum::Chase)
    {
        skeletalController->StopMovement();
        FinishLatentTask(ownerComp, EBTNodeResult::Failed);
        return;
    }
    ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
    UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
    if (skeleton->bDamaged)
    {
        currentState = EAIStateEnum::Damaged;
        blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
        FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);

    }
    if (currentState != EAIStateEnum::Chase)
    {
        FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
        return;
    }
    if (targetActor)
    {
        FVector currentTargetLoc = targetActor->GetActorLocation();
        skeletalController->MoveToLocation(currentTargetLoc);
    }

}




