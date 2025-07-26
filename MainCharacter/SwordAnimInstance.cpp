// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../SpawnActor/SwordActor.h"
#include "../SpawnActor/AOEAttackCollisionActor.h"
#include "../SpawnActor/DownwardknockbackActor.h"
#include "SwordPlayController.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h" 
#include "Containers/Array.h"
#include "SwordCharacter.h"

void USwordAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	swordCharacter = Cast<ASwordCharacter>(TryGetPawnOwner());
	

	

	if (swordCharacter)
	{
		owingMovementComponent = swordCharacter->GetCharacterMovement();

		//character lookdirection
		forwardDirection = swordCharacter->GetMesh()->GetForwardVector();

		launchForwardDirection = forwardDirection * 1500.0f;
	}
	
}

void USwordAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{

	if (swordCharacter != nullptr)
	{
		GroundSpeed = swordCharacter->GetVelocity().Size2D();

		if (GroundSpeed > 0.f)
		{
			BHasAcceleration = true;
		}		

		
		
	}
	
}
void USwordAnimInstance::AnimNotify_ESkillJumpStart()
{
	
	//first speed and angle
	float firstSpeed = 1700.0f;
	float firstLaunchAngle = 45.0f;

	//angle to radian change
	float radianAngle = FMath::DegreesToRadians(firstLaunchAngle);
	FVector skillEJumpDirection = forwardDirection * firstSpeed * FMath::Cos(radianAngle); // 수평 방향
	skillEJumpDirection.Z = firstSpeed * FMath::Sin(radianAngle); // 위로 가는 속도

	swordCharacter->LaunchCharacter(skillEJumpDirection, true, true);
	if (swordCharacter->bForceMode == false)
	{
		swordCharacter->spawnOriginGreatSword();
	}
	if (swordCharacter->bForceMode)
	{
		swordCharacter->spawnForceGreatSword();
	}
}

void USwordAnimInstance::AnimNotify_ESkillJumpEnd()
{
	FVector oppositeDirection = -forwardDirection; 
	float downSpeed = 2000.0f; 
	FVector skillThirdMoveDirection = oppositeDirection.GetSafeNormal() * downSpeed;
	skillThirdMoveDirection.Z = -downSpeed * 1.2f;
	swordCharacter->LaunchCharacter(skillThirdMoveDirection, true, true);

	FVector spawnLoc = swordCharacter->GetActorLocation() + swordCharacter->GetActorForwardVector() * 500.0f;
	FRotator spawnRot = swordCharacter->GetActorRotation();


	

}

void USwordAnimInstance::AnimNotify_WForceSlowStart()
{
	swordCharacter->cameraWZoomIn();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.15f);
	
}

void USwordAnimInstance::AnimNotify_WForceSlowEnd()
{
	swordCharacter->cameraWZoomOut();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	
}

void USwordAnimInstance::AnimNotify_RForceCameraShake()
{
	swordCharacter->cameraShakeR();

}
void USwordAnimInstance::AnimNotify_AForceCameraShake()
{
	swordCharacter->cameraShakeQ();
}
void USwordAnimInstance::AnimNotify_EGreatSwordHitNormal()
{
	swordCharacter->CameraShakeEGreatSwordNormal();

}
void USwordAnimInstance::AnimNotify_EGreatSwordHitForce()
{
	swordCharacter->CameraShakeEGreatSwordForce();

}

void USwordAnimInstance::AnimNotify_AttackCollisionActivate()
{
	swordCharacter->swordActorInstance->SetEnableAttackCollision();
	
	
}

void USwordAnimInstance::AnimNotify_AttackCollisionNonActivate()
{
	swordCharacter->swordActorInstance->SetDisableAttackCollision();
	
}

void USwordAnimInstance::AnimNotify_SkillQAoeAttack()
{
	FVector spawnLoc = swordCharacter->GetActorLocation() + FVector(50.0f, 0.f, 0.f);
	FRotator spawnRot = swordCharacter->GetActorRotation();

	
	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(4.0f, 4.0f, 0.5f));
		spawnedActor->SetLifeSpan(0.35f);
	}

}

void USwordAnimInstance::AnimNotify_SkillRAoeAttack()
{
	forceN = 0;
	FVector spawnLoc = swordCharacter->GetActorLocation() + FVector(10.0f, 0.f, 0.f);
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(4.0f, 4.0f, 0.5f));
		spawnedActor->SetLifeSpan(0.35f);
	}
}

void USwordAnimInstance::AnimNotify_SkillEAoeAttack()
{
	FVector spawnLoc = swordCharacter->GetActorLocation() + swordCharacter->GetActorForwardVector() * 100.0f;
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(2.5f, 2.5f, 0.7f));
		spawnedActor->SetLifeSpan(0.35f);
	}
}

void USwordAnimInstance::AnimNotify_SkillWAoeAttack()
{
	FVector spawnLoc = swordCharacter->GetActorLocation() + FVector(50.0f, 0.f, 0.f);
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(4.0f, 4.0f, 0.5f));
		spawnedActor->SetLifeSpan(0.35f);
	}
}

void USwordAnimInstance::AnimNotify_SkillESwordBigAttack()
{
	forceN = 3;

	FVector spawnLoc = swordCharacter->GetActorLocation() + swordCharacter->GetActorForwardVector() * 400.0f; 
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);
	
	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(3.375f, 3.5f, 2.125f));
		spawnedActor->SetLifeSpan(0.35f);
	}
	FVector spawnKnockBackActorLoc = spawnLoc + FVector(0.f, 0.f, -100.f) - swordCharacter->GetActorForwardVector() * 150.f;
	ADownwardknockbackActor* spawnedKnockBackActor = GetWorld()->SpawnActor<ADownwardknockbackActor>(knockBackClass, spawnKnockBackActorLoc, spawnRot);
	if (spawnedKnockBackActor)
	{
		spawnedKnockBackActor->SetActorScale3D(FVector(5.0f));
		spawnedKnockBackActor->SetLifeSpan(0.35f);
	}
}

void USwordAnimInstance::AnimNotify_SkillESwordBigAttackForceVer()
{
	forceN = 4;
	FVector spawnLoc = swordCharacter->GetActorLocation() + swordCharacter->GetActorForwardVector() * 600.0f;
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(4.875f, 5.0625f, 2.125f));
		spawnedActor->SetLifeSpan(0.35f);
	}
	FVector spawnKnockBackActorLoc = spawnLoc + FVector(0.f, 0.f, -100.f) - swordCharacter->GetActorForwardVector() * 250.f;
	ADownwardknockbackActor* spawnedKnockBackActor = GetWorld()->SpawnActor<ADownwardknockbackActor>(knockBackClass, spawnKnockBackActorLoc, spawnRot);
	if (spawnedKnockBackActor)
	{
		spawnedKnockBackActor->SetActorScale3D(FVector(6.875f));
		spawnedKnockBackActor->SetLifeSpan(0.35f);
	}
}








void USwordAnimInstance::AnimNotify_RForceLastSlash()
{
	swordCharacter->spawnRLastSlash();
	forceN = 1;

	FVector spawnLoc = swordCharacter->GetActorLocation() + FVector(10.0f, 0.f, 0.f);
	FRotator spawnRot = swordCharacter->GetActorRotation();


	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(5.0f, 5.0f, 0.8f));
		spawnedActor->SetLifeSpan(0.35f);
	}

}

void USwordAnimInstance::AnimNotify_SkillEMotionWarpingStart()
{
	swordCharacter->MotionWarpingMouseCursor();
}

void USwordAnimInstance::AnimNotify_SwordSetOriginPosition()
{
	swordCharacter->overlapSword->SetRelativeLocation(FVector(0.0f));
	swordCharacter->overlapSword->SetRelativeRotation(FRotator(0.0f));
}

void USwordAnimInstance::AnimNotify_SwordAChangePosition()
{
	swordCharacter->overlapSword->SetRelativeLocation(FVector(21.756625f, -0.044639f, 1.828127f));
	swordCharacter->overlapSword->SetRelativeRotation(FRotator(-4.303001f, 176.927141f, -183.068815f));
}

void USwordAnimInstance::AnimNotify_SwordSChangePosition()
{
	swordCharacter->overlapSword->SetRelativeRotation(FRotator(-12.767403f, -30.123265f, -1.251592f));
}

void USwordAnimInstance::AnimNotify_SwordAZoomFarAway()
{
	swordCharacter->cameraFarAway();
}

void USwordAnimInstance::AnimNotify_SwordAZoomOrigin()
{
	swordCharacter->cameraSetOrigin();
}

void USwordAnimInstance::AnimNotify_SpawnSwordSlash()
{
	swordCharacter->SpawnSwordSlash();
}

void USwordAnimInstance::AnimNotify_CharacterInvisible()
{
	swordCharacter->GetMesh()->SetVisibility(false);
	swordCharacter->overlapSword->SetVisibility(false);
}

void USwordAnimInstance::AnimNotify_CharacterVisible()
{
	swordCharacter->GetMesh()->SetVisibility(true);
	swordCharacter->overlapSword->SetVisibility(true);
}

void USwordAnimInstance::AnimNotify_SSkillMoving()
{
	swordCharacter->StartSSkillMove();
	swordCharacter->spawnSkillSSlashNiagara();

	FVector spawnLoc = swordCharacter->GetActorLocation() + swordCharacter->GetActorForwardVector() * 600;
	FRotator spawnRot = swordCharacter->GetActorRotation();

	AAOEAttackCollisionActor* spawnedActor = GetWorld()->SpawnActor<AAOEAttackCollisionActor>(aOEActorClass, spawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(11.0f, 2.0f, 0.8f));
		spawnedActor->SetLifeSpan(0.35f);
	}
}

void USwordAnimInstance::AnimNotify_RadialBlurStart()
{
	swordCharacter->StartRadialBlur();
	
}

void USwordAnimInstance::AnimNotify_RadialBlurEnd()
{
	swordCharacter->EndRadialBlur();
}

void USwordAnimInstance::AnimNotify_IgnorePawn()
{
	swordCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void USwordAnimInstance::AnimNotify_SetCollisionOrigin()
{
	swordCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	
}

void USwordAnimInstance::AnimNotify_SpawnKnockbackCollision()
{
	swordCharacter->SpawnIgnoreKnockBackActor();
}

void USwordAnimInstance::AnimNotify_ShowGhostTrail()
{
	swordCharacter->ShowGhostTrail();

	
}

void USwordAnimInstance::AnimNotify_HideGhostTrail()
{
	swordCharacter->HideGhostTrail();
	
}

void USwordAnimInstance::AnimNotify_SwordSwitch()
{
	swordCharacter->overlapSword->SetVisibility(true);
}

void USwordAnimInstance::AnimNotify_FootStep()
{
	swordCharacter->FootStep();
}

void USwordAnimInstance::AnimNotify_Unvisible()
{
	swordCharacter->PlayerUnvisible();
}


void USwordAnimInstance::AnimNotify_DestroyPlayerPortal()
{
	swordCharacter->DestroyPlayerPortal();
}
