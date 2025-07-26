// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwordCharacter.h"
#include "../SpawnActor/DownwardknockbackActor.h"
#include "Sound/SoundBase.h" 
#include "Containers/Array.h" 
#include "SwordAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USwordAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ASwordCharacter* swordCharacter;

	UPROPERTY()
	UCharacterMovementComponent* owingMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Animation|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Animation|LocomotionData")
	bool BHasAcceleration;
	
	

	UFUNCTION()
	void AnimNotify_ESkillJumpStart();


	UFUNCTION()
	void AnimNotify_ESkillJumpEnd();

	UFUNCTION()
	void AnimNotify_WForceSlowStart();

	UFUNCTION()
	void AnimNotify_WForceSlowEnd();

	UFUNCTION()
	void AnimNotify_RForceLastSlash();

	int forceN;

	UFUNCTION()
	void AnimNotify_RForceCameraShake();

	UFUNCTION()
	void AnimNotify_AForceCameraShake();

	UFUNCTION()
	void AnimNotify_EGreatSwordHitNormal();

	UFUNCTION()
	void AnimNotify_EGreatSwordHitForce();

	UFUNCTION()
	void AnimNotify_AttackCollisionActivate();

	UFUNCTION()
	void AnimNotify_AttackCollisionNonActivate();

	UFUNCTION()
	void AnimNotify_SkillQAoeAttack();

	UFUNCTION()
	void AnimNotify_SkillRAoeAttack();

	UFUNCTION()
	void AnimNotify_SkillEAoeAttack();

	UFUNCTION()
	void AnimNotify_SkillWAoeAttack();

	UFUNCTION()
	void AnimNotify_SkillESwordBigAttack();

	UFUNCTION()
	void AnimNotify_SkillESwordBigAttackForceVer();

	//
	UFUNCTION()
	void AnimNotify_SkillEMotionWarpingStart();

	UFUNCTION()
	void AnimNotify_SwordSetOriginPosition();

	UFUNCTION()
	void AnimNotify_SwordAChangePosition();

	UFUNCTION()
	void AnimNotify_SwordSChangePosition();

	UFUNCTION()
	void AnimNotify_SwordAZoomFarAway();

	UFUNCTION()
	void AnimNotify_SwordAZoomOrigin();

	UFUNCTION()
	void AnimNotify_SpawnSwordSlash();

	UFUNCTION()
	void AnimNotify_CharacterInvisible();

	UFUNCTION()
	void AnimNotify_CharacterVisible();

	UFUNCTION()
	void AnimNotify_SSkillMoving();

	UFUNCTION()
	void AnimNotify_RadialBlurStart();

	UFUNCTION()
	void AnimNotify_RadialBlurEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class AAOEAttackCollisionActor> aOEActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class ADownwardknockbackActor> knockBackClass;

	FVector forwardDirection;
	FVector launchForwardDirection;



	class ASwordPlayController* swordPlayerController;

	//ignore collision notify
	UFUNCTION()
	void AnimNotify_IgnorePawn();

	UFUNCTION()
	void AnimNotify_SetCollisionOrigin();

	UFUNCTION()
	void AnimNotify_SpawnKnockbackCollision();

	//ghost trail
	UFUNCTION()
	void AnimNotify_ShowGhostTrail();

	UFUNCTION()
	void AnimNotify_HideGhostTrail();

	//sequence
	UFUNCTION()
	void AnimNotify_SwordSwitch();

	//foot step sound
	UFUNCTION()
	void AnimNotify_FootStep();

	//polymorp
	UFUNCTION()
	void AnimNotify_Unvisible();

	UFUNCTION()
	void AnimNotify_DestroyPlayerPortal();
	
};
