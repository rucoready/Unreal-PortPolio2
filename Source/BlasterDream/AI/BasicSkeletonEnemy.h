// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkeletonAIController.h"
#include "../MainCharacter/HitInterface.h"
#include "../MainCharacter/CharacterWidget/DamageWidget.h"
#include "EnemyUIDisplayWidget.h"
#include "HitTestCrow.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BowSkeletonProjectTile.h"
#include "BasicSkeletonEnemy.generated.h"

UCLASS()
class BLASTERDREAM_API ABasicSkeletonEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABasicSkeletonEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	UPROPERTY(EditAnywhere, Category="Sword")
	USkeletalMeshComponent* sword;

	UPROPERTY(EditAnywhere, Category = "Sword")
	UCapsuleComponent* swordCollision;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ASwordCharacter* swordCharacter;

	UFUNCTION()
	void OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* attack01;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* attack02;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* attack03;

	UFUNCTION()
	void PlayAttackAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> motionWarpComponent;


	ASkeletonAIController* aiController;

	UFUNCTION()
	void MotionWarpingPlayer();

	UFUNCTION()
	void CollisionOff();

	UFUNCTION()
	void CollisionOn();

	bool bHit;

	
	//HUD
	UPROPERTY(EditAnywhere, Category = "Hud")
	class UWidgetComponent* damageWidget;

	UDamageWidget* damageWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Popup")
	TSubclassOf<AActor> damagePopupActor;

	UFUNCTION()
	void ShowHud(float damage);

	virtual void GetHit(const FVector& ImpactPoint)override;

	UPROPERTY(EditAnywhere, Category = "damage")
	float attackDamage;

	//viewport
	//display
	FString monsterName = "Skeleton";
	//hp
	float currentHp;
	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxHp = 700.0f;
	float damageAmount;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEnemyUIDisplayWidget> enemyDisplayWidget;

	static UEnemyUIDisplayWidget* enemyUIDisplayWidgetInstance;
	static ABasicSkeletonEnemy* currentWidgetOwner;

	FTimerHandle th_HideUIAfterDelay;

	UFUNCTION()
	void SkeletonBeAttacked();

	UFUNCTION()
	void DamagedMontagePlay();



	UPROPERTY(EditAnywhere, Category="Damaged")
	UAnimMontage* damagedMontage;

	class AHitTestCrow* crow;

	bool bDamaged;

	int32 pendingDamageCount = 0;

	UFUNCTION()	
	void OnDamagedMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, Category = "TREE")
	class UBehaviorTree* behaviorTree;

	UPROPERTY(EditAnywhere, Category = "TREE")
	class UBlackboardData* blackBord;

	UPROPERTY(EditAnywhere, Category = "TREE")
	class UBlackboardComponent* blackBoardComponent;

	UPROPERTY(EditAnywhere, Category = "TREE")
	UBehaviorTreeComponent* behaviorTreeComponent;

	// BlackboardComponent를 반환하는 함수
	UBlackboardComponent* GetBlackboardComponent() 
	{
		return blackBoardComponent;
	}

	UBehaviorTreeComponent* GetBehaviorTreeComponent() 
	{
		return behaviorTreeComponent;
	}

	// BlackboardData와 BehaviorTree를 반환하는 함수들
	UBehaviorTree* GetBehaviorTree() const { return behaviorTree; }
	UBlackboardData* GetBlackboard() const { return blackBord; }

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UWidgetComponent* hpFloatingWidget;

	class USkeletonHPBarWidget* skeletonHpBarWidget;

	UFUNCTION()
	void SpawnArrow();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<ABowSkeletonProjectTile> arrowProjectTile;

	UPROPERTY()
	ASwordCharacter* lastHitSwordCharacter = nullptr;
};
