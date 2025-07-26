// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DownwardknockbackActor.generated.h"

UCLASS()
class BLASTERDREAM_API ADownwardknockbackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADownwardknockbackActor();

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USphereComponent* collisionSphere;

	UFUNCTION()
	void OnBeginOverlapSphereCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, Category = "MySettings")
	TArray<AActor*> ignoreActor;

	class UStatGameInstance* statGI;
	class AHitTestCrow* crow;
	class ABasicSkeletonEnemy* skeleton;

	float aBaseDamage;
	float aBaseLastDamage;

	UFUNCTION()
	void getDamageDatableValue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float calculateDamage;

	UPROPERTY(EditAnywhere, Category="MySettings")
	float launchForce;

};
