// Fill out your copyright notice in the Description page of Project Settings.


#include "DrangonAnimInstance.h"
#include "../SpawnActor/BiteAttackCollision.h"
#include "../SpawnActor/AOEAttackCollisionActor.h"


void UDrangonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	dragonCharacter = Cast<ADragonCharacter>(TryGetPawnOwner());
}

void UDrangonAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (dragonCharacter != nullptr)
	{
		groundSpeed = dragonCharacter->GetVelocity().Size2D();

		if (groundSpeed > 0.f)
		{
			bHasAcceleration = true;
		}
		angle = dragonCharacter->GetLookAngle();
	}

	
}

void UDrangonAnimInstance::AnimNotify_TakeOff()
{
	dragonCharacter->OnTakeOff();
}

void UDrangonAnimInstance::AnimNotify_TakeOn()
{
	dragonCharacter->OnTakeOn();
}

void UDrangonAnimInstance::AnimNotify_FlyingStart()
{
	dragonCharacter->DragonFlying();
}

void UDrangonAnimInstance::AnimNotify_FlyingStop()
{
	dragonCharacter->DragonStopFlying();
}

void UDrangonAnimInstance::AnimNotify_Bite()
{
	FVector biteSpawnLoc = dragonCharacter->GetMesh()->GetSocketLocation(TEXT("FireSocket"));
	FRotator spawnRot = dragonCharacter->GetActorRotation();


	ABiteAttackCollision* spawnedActor = GetWorld()->SpawnActor<ABiteAttackCollision>(biteCollisionClass, biteSpawnLoc, spawnRot);

	if (spawnedActor)
	{
		spawnedActor->SetActorScale3D(FVector(2.0f));
		spawnedActor->SetLifeSpan(0.1f);
	}
}

void UDrangonAnimInstance::AnimNotify_Visible()
{
	dragonCharacter->DragonVisible();
}

void UDrangonAnimInstance::AnimNotify_UnVisible()
{
	dragonCharacter->DragonUnVisible();
}

void UDrangonAnimInstance::AnimNotify_PolyCancle()
{
	if (dragonCharacter)
	{
		dragonCharacter->PolyMolfCancleAnimationFinished();
	}
}







