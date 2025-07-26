// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DragonCharacter.h"
#include "DrangonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UDrangonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ADragonCharacter* dragonCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	float groundSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Animation|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bGliding = false;

	UFUNCTION()
	void AnimNotify_TakeOff();

	UFUNCTION()
	void AnimNotify_TakeOn();

	UFUNCTION()
	void AnimNotify_FlyingStart();

	UFUNCTION()
	void AnimNotify_FlyingStop();

	UFUNCTION()
	void AnimNotify_Bite();

	UFUNCTION()
	void AnimNotify_Visible();

	UFUNCTION()
	void AnimNotify_UnVisible();

	UFUNCTION()
	void AnimNotify_PolyCancle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class ABiteAttackCollision> biteCollisionClass;

	//bs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bWAiming = false;
};
