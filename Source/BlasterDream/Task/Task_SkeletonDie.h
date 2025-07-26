// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_SkeletonDie.generated.h"

/**
 * 
 */
USTRUCT()
struct FTaskSkeletonDieMemory
{
	GENERATED_BODY()

	float structCurrentTime = 0.0f;
};

UCLASS()
class BLASTERDREAM_API UTask_SkeletonDie : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_SkeletonDie();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	float minDissolveValue;
	float maxDissolveValue;

	class ASwordPlayerGameBase* gameMode;
	class UStatGameInstance* myInstance;

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<AActor> moneyActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> gearActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> potionActorHp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> potionActorMp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> potionActorForce;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> potionActorElic;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> potionActorP;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* dieSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor> soulActor;
};
