
#include "SkeletonAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree//BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../Task//AIStateEnum.h"
#include "Components/WidgetComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "Kismet/GameplayStatics.h"
#include "BasicSkeletonEnemy.h"



ASkeletonAIController::ASkeletonAIController()
{
	//BT
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(TEXT("/Game/Luco/Enemy/BT_Skeleton.BT_Skeleton"));
	if (bt.Succeeded())
	{
		behaviorTreeAsset = bt.Object;
	}
	//BB
	static ConstructorHelpers::FObjectFinder<UBlackboardData> bb(TEXT("/Game/Luco/Enemy/BB_Skeleton.BB_Skeleton"));
	if (bb.Succeeded())
	{
		blackboardData = bb.Object;
	}
	perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");

	sight->SightRadius = 600; //sense distance
	sight->LoseSightRadius = 800; //lostsense distance
	sight->PeripheralVisionAngleDegrees = 180; //sense degree
	
	//sense enemy
	sight->DetectionByAffiliation.bDetectEnemies = true; 
	//no sense neutrality
	sight->DetectionByAffiliation.bDetectNeutrals = false;
	//sense friend
	sight->DetectionByAffiliation.bDetectFriendlies = false;

	perception->SetDominantSense(*sight->GetSenseImplementation());
	perception->ConfigureSense(*sight);

	//perception update
	perception->OnPerceptionUpdated.AddDynamic(this, &ASkeletonAIController::OnPerceptionUpdated);

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}


void ASkeletonAIController::BeginPlay()
{
	Super::BeginPlay();
	EnablePerception();
}

void ASkeletonAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//skeleton = Cast<ABasicSkeletonEnemy>(InPawn);
	if (ABasicSkeletonEnemy* const skeletons = Cast<ABasicSkeletonEnemy>(InPawn))
	{
		if (UseBlackboard(blackboardData, BlackboardComp)) 
		{
			RunBehaviorTree(behaviorTreeAsset);      
		}
	}

	
}

void ASkeletonAIController::OnPerceptionUpdated(const TArray<AActor*>& DetectPawn)
{
	if (!perception->IsActive()) return;
	TArray<AActor*> actors;
	perception->GetCurrentlyPerceivedActors(nullptr, actors);
	skeleton = Cast<ABasicSkeletonEnemy>(GetPawn());

	for (AActor* actor : actors)
	{
		APawn* perceivedPawn = Cast<APawn>(actor);
		if (perceivedPawn)
		{
			Blackboard->SetValueAsObject("Target", perceivedPawn);
			RunEQS();
			skeleton->hpFloatingWidget->SetVisibility(true);
			return;
		}
	}
	skeleton->hpFloatingWidget->SetVisibility(false);
	Blackboard->SetValueAsObject("Target", nullptr);
}

void ASkeletonAIController::DisablePerception()
{
	if (perception)
	{
		perception->SetActive(false);
	}
	if (Blackboard)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
	}
}

void ASkeletonAIController::EnablePerception()
{
	if (perception)
	{
		perception->SetActive(true);
	}

	TArray<AActor*> actors;
	perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (AActor* actor : actors)
	{
		APawn* perceivedPawn = Cast<APawn>(actor);
		if (perceivedPawn)
		{
			Blackboard->SetValueAsObject("Target", perceivedPawn);
	
			return;
		}
	}

}





ETeamAttitude::Type ASkeletonAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* otherTeamID = Cast<IGenericTeamAgentInterface>(&Other);
	if (otherTeamID)
	{
		// enemy team id == 1
		if (otherTeamID->GetGenericTeamId() == FGenericTeamId(1)) // player == 1
		{
			return ETeamAttitude::Hostile;  // enemy
		}
		else
		{
			return ETeamAttitude::Neutral;  // each ai neutral
		}
	}

	// if IGenericTeamAgentInterface Not Have == neutral
	return ETeamAttitude::Neutral;
}

FGenericTeamId ASkeletonAIController::GetGenericTeamId() const
{
	return FGenericTeamId(2);
}

void ASkeletonAIController::RunEQS()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this,              
		FindTargetQuery,  
		this,             
		EEnvQueryRunMode::AllMatching, 
		UEnvQueryInstanceBlueprintWrapper::StaticClass() 
	);
	
}

void ASkeletonAIController::StopAI()
{
	UBehaviorTreeComponent* behaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == behaviorTreeComponent) return;
	behaviorTreeComponent->StopTree(EBTStopMode::Safe);
}



