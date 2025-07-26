// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "DragonWOrb.generated.h"

UCLASS()
class BLASTERDREAM_API ADragonWOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	ADragonWOrb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UCapsuleComponent* collisionComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UNiagaraComponent* niagaraComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UProjectileMovementComponent* projectileMovement;

	float currentScale = 1.f;
	float targetScale = 3.f;
	float grownDuration = 0.5f;
	float elaspedTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bFire;

	UFUNCTION()
	void OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector targetLocation;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor>explosionActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	UParticleSystem* explosionParticle;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* energyHumSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	USoundBase* chargeEndSound;

	UAudioComponent* humSoundComp = nullptr;

	bool bOnceChargeSound = false;



};
