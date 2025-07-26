// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../MainCharacter/CharacterWidget/DamageWidget.h"
#include "Components/WidgetComponent.h"
#include "DamagePopupActor.generated.h"

UCLASS()
class BLASTERDREAM_API ADamagePopupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagePopupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitDamage(float damage, bool bCritical = false);

	UPROPERTY(EditAnywhere, Category="MyHudSettings")
	UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, Category = "MyHudSettings")
	UDamageWidget* damageWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* critSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* normalSound;

};
