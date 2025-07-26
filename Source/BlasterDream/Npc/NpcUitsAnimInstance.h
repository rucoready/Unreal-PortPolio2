// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NpcUits.h"
#include "NpcUitsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()

class BLASTERDREAM_API UNpcUitsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ANpcUits* npcCharacter;


};
