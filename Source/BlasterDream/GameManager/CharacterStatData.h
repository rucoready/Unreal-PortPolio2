// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float normalAttackBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float normalAttackLastBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillQBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillQLastBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillWBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillWLastBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillEBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillELastBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillRBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillRLastBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillABaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float skillALastBaseDamage;
	
	FCharacterStatData()
		: normalAttackBaseDamage(0.f)
		, normalAttackLastBaseDamage(0.f)
		, skillQBaseDamage(0.f)
		, skillQLastBaseDamage(0.f)
		, skillWBaseDamage(0.f)
		, skillWLastBaseDamage(0.f)
		, skillEBaseDamage(0.f)
		, skillELastBaseDamage(0.f)
		, skillRBaseDamage(0.f)
		, skillRLastBaseDamage(0.f)
		, skillABaseDamage(0.f)
		, skillALastBaseDamage(0.f)
	{}
	
};
