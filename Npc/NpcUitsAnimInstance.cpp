// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcUitsAnimInstance.h"
#include "NpcUits.h"

void UNpcUitsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	npcCharacter = Cast<ANpcUits>(TryGetPawnOwner());

}

void UNpcUitsAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}
