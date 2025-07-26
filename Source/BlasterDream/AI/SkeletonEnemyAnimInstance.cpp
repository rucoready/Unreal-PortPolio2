// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemyAnimInstance.h"
#include "BasicSkeletonEnemy.h"


void USkeletonEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	skeletonEnemy = Cast<ABasicSkeletonEnemy>(TryGetPawnOwner());
}

void USkeletonEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (skeletonEnemy)
	{
		groundSpeed = skeletonEnemy->GetVelocity().Size2D();
		FVector velocity = skeletonEnemy->GetVelocity();
		direction = FMath::FindDeltaAngleDegrees(skeletonEnemy->GetActorRotation().Yaw, velocity.ToOrientationRotator().Yaw);
	}

	
	
}

void USkeletonEnemyAnimInstance::AnimNotify_CollisionOn()
{
	skeletonEnemy->CollisionOn();
}

void USkeletonEnemyAnimInstance::AnimNotify_CollisionOff()
{
	skeletonEnemy->CollisionOff();
}

void USkeletonEnemyAnimInstance::AnimNotify_SpawnArrow()
{
	skeletonEnemy->SpawnArrow();
}


