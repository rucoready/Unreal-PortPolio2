

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "SkeletonAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API ASkeletonAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASkeletonAIController();

	virtual void OnPossess(APawn* InPawn) override;
	void StopAI();



	


	virtual void BeginPlay() override;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComp;

	// 에디터에서 설정해야 할 에셋 (공용 블랙보드 데이터와 BT)
	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* blackboardData;

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* behaviorTreeAsset;

	//Perception
	UPROPERTY(EditAnywhere, Category="Perceptions")
	class UAIPerceptionComponent* perception;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& DetectPawn);

	UPROPERTY()
	bool bActivePerception = true;

	UFUNCTION()
	void DisablePerception();

	UFUNCTION()
	void EnablePerception();

	class UAISenseConfig_Sight* sight;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	

	class ABasicSkeletonEnemy* skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
	class UEnvQuery* FindTargetQuery;

	UFUNCTION()
	void RunEQS();


};
