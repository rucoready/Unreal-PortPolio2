// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEAttackCollisionActor.generated.h"

UCLASS()
class BLASTERDREAM_API AAOEAttackCollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOEAttackCollisionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UBoxComponent* collisionBox;

	UFUNCTION()
	void OnBeginOverlapBoxCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TArray<AActor*> ignoreActor;


	float normalLastAttackDamage;
	float qAttackLastDamage;
	float wAttackLastDamage;
	float wAttackDamage;
	float eAttackDamage;
	float eAttackLastDamage;
	float rAttackLastDamage;
	float rAttackDamage;

	class UStatGameInstance* statGI;

	class USwordAnimInstance* animIT;

	UFUNCTION()
	void getDamageDatableValue();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ASwordCharacter* swordCharacter;

	class AHitTestCrow* crow;
	class ABasicSkeletonEnemy* skeleton;

	class UAnimInstance* animInstance;

	float calculateDamage;
};
