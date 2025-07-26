// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_SmartSkeleton.h"
#include "../AI/BasicSkeletonEnemy.h"
#include "../AI/SkeletonAIController.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/EnvQueryContext.h"  



void UEQC_SmartSkeleton::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    Super::ProvideContext(QueryInstance, ContextData);

    UObject* querierOBJ = QueryInstance.Owner.Get();
    AActor* querierActor = Cast<AActor>(querierOBJ);
    AAIController* aICon = Cast<AAIController>(querierActor);
    UBlackboardComponent* blackboard = nullptr;

    if (aICon)
    {
        blackboard = aICon->GetBlackboardComponent();
    }
    else
    {
        APawn* usePawn = Cast<APawn>(querierActor);
        if (usePawn)
        {
            AAIController* ownerCon = Cast<AAIController>(usePawn->GetController());
            if (ownerCon)
            {
                blackboard = ownerCon->GetBlackboardComponent();
            }
        }
    }

    if (blackboard)
    {
        //Target == Player
        AActor* targetActor = Cast<AActor>(blackboard->GetValueAsObject("Target"));
        if (targetActor)
        {
            FVector targetLocation = targetActor->GetActorLocation();
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, targetActor);          
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("TargetActor is valid!"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TargetActor is Null"));
        }
    }
   
}
