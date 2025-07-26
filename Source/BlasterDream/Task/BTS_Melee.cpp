// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Melee.h"
#include "../AI/SkeletonAIController.h"
#include "../AI//BasicSkeletonEnemy.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "../AI/BasicSkeletonEnemy.h"
#include "AIStateEnum.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacter/DragonCharacter.h"
#include "../MainCharacter/SwordCharacter.h"


UBTS_Melee::UBTS_Melee()
{
	NodeName = "State Service";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UBTS_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* blackBoard = OwnerComp.GetBlackboardComponent();
	APlayerController* playerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* currentUsePlayerPawn = playerCon->GetPawn();
	ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UObject* targetObj = blackBoard->GetValueAsObject("Target");

	//°»½Å
	if (targetObj != currentUsePlayerPawn)
	{
		blackBoard->SetValueAsObject("Target", currentUsePlayerPawn);
		targetObj = currentUsePlayerPawn;
	}

	ASwordCharacter* targetCharacter = Cast<ASwordCharacter>(targetObj);
	ADragonCharacter* dragonCharacter = Cast<ADragonCharacter>(targetObj);
	
	FVector skeletonLoc = skeleton->GetActorLocation();
	if (skeleton->currentHp <= 0.0f)
	{
		currentState = EAIStateEnum::Die;
		blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
		return;
	}
	
	if (skeleton->bDamaged)
	{
		currentState = EAIStateEnum::Damaged;
		blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
		
		//skeleton->bDamaged = false;
		
	}
	
	else
	{
		if (targetObj == nullptr)
		{
			currentState = EAIStateEnum::OutOfSight;
			blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
			return;
		}

		if (targetCharacter)
		{

			playerLoc = targetCharacter->GetActorLocation();

			blackBoard->SetValueAsVector("PlayerLocation", playerLoc);

			//set state enum class
			float distance = FVector::Dist(playerLoc, skeletonLoc);
			blackBoard->SetValueAsFloat("Dist", distance);

			if (distance <= attackDistance)
			{
				currentState = EAIStateEnum::Attack;
				blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
			}
			else
			{
				currentState = EAIStateEnum::Chase;
				blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
			}
		}
		if(dragonCharacter)
		{
			playerLoc = dragonCharacter->GetActorLocation();

			blackBoard->SetValueAsVector("PlayerLocation", playerLoc);

			//set state enum class
			float distance = FVector::Dist(playerLoc, skeletonLoc);
			blackBoard->SetValueAsFloat("Dist", distance);

			if (distance <= dragonAttackDistance)
			{
				currentState = EAIStateEnum::Attack;
				blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
			}
			else
			{
				currentState = EAIStateEnum::Chase;
				blackBoard->SetValueAsEnum("AIState", static_cast<uint8>(currentState));
			}
		}
	}
}



