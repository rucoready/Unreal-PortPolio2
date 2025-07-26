// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordActor.generated.h"

UCLASS()
class BLASTERDREAM_API ASwordActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class USceneComponent* sceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class UStaticMeshComponent* swordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class UCapsuleComponent* swordCapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class USceneComponent* capsureTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class USceneComponent* capsureTraceEndPoint;

	UFUNCTION()
	void OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bSwordAttackCollisionActivate;

	UFUNCTION()
	void SetEnableAttackCollision();

	UFUNCTION()
	void SetDisableAttackCollision();

	UPROPERTY()
	TArray<AActor*> ignoreActors;

	void emptyIgnoreActorArr();

	////display
	UFUNCTION()
	void getMyCharacterSkillType(FString skill, float damage);

	FString skillName;
	float skillDamage;

	float normalAttackDamage;
	float normalLastAttackDamage;
	float qAttackDamage;
	float qAttackLastDamage;
	float wAttackDamage;
	float wAttackLastDamage;
	float eAttackDamage;
	float eAttackLastDamage;
	float rAttackDamage;
	float rAttackLastDamage;
	
	class AHitTestCrow* crow;
	class ABasicSkeletonEnemy* skeleton;
	
	
	UFUNCTION()
	void ClearSkillName();

	UPROPERTY(EditAnywhere, Category="MySettings")
	class ASwordCharacter* swordCharacter;

	class UStatGameInstance* statGI;

	UFUNCTION()
	void getDamageDatableValue();

	float randomPlusNum;

	float calculateDamage;
	

	
};
