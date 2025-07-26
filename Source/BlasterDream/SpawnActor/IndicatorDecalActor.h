// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "IndicatorDecalActor.generated.h"

/**
 *
 */
UCLASS()
class BLASTERDREAM_API AIndicatorDecalActor : public ADecalActor
{
	GENERATED_BODY()

public:
	AIndicatorDecalActor();
	virtual void BeginPlay() override;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	void setMPCScalar1Value();
	void setMPCScalar0Value();
	void cacheOriginalBlueColor();
	void forceSetBlue();

	FLinearColor g_OriginalBlueColor = FLinearColor::Transparent;

};
