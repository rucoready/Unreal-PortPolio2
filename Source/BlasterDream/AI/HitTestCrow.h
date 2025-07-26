// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../MainCharacter/HitInterface.h"
#include "../MainCharacter/CharacterWidget/DamageWidget.h"
#include "EnemyUIDisplayWidget.h"
#include "HitTestCrow.generated.h"

UCLASS()
class BLASTERDREAM_API AHitTestCrow : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHitTestCrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class USceneComponent* sceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MySword")
	class UStaticMeshComponent* scareCrowMesh;

	virtual void GetHit(const FVector& ImpactPoint)override;

	UFUNCTION()
	void DummyBeAttacked();

	UMaterialInterface* originMat;

	UPROPERTY()
	UMaterialInstanceDynamic* dynamicMat;
	
	FTimerHandle th_HandlePitch10Plus;

	FTimerHandle th_HandleChangeOriginMat;

	float currentHitAmount = 0.f;
	float targetHitAmount = 0.f;
	float hitLerpSpeed = 15.0f;

	FRotator originRot;

	//HUD
	UPROPERTY(EditAnywhere, Category = "Hud")
	class UWidgetComponent* damageWidget;

	UDamageWidget* damageWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Popup")
	TSubclassOf<AActor> damagePopupActor;

	void ShowHud(float damage);
	void TakeDamage();

	//display
	FString monsterName = "Dummy";
	float currentHp;
	float maxHp = 9999.0f;

	float damageAmount;


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEnemyUIDisplayWidget> enemyDisplayWidget;

	static UEnemyUIDisplayWidget* enemyUIDisplayWidgetInstance;
	static AHitTestCrow* currentWidgetOwner;

	FTimerHandle th_HideUIAfterDelay;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class ASwordCharacter* swordCharacter;

	//training
	FVector originSetLocation;
	FRotator originSetRotation;
};
