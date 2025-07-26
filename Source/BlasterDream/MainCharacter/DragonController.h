// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DrangonAnimInstance.h"
#include "DragonController.generated.h"


/**
 * 
 */
UCLASS()
class BLASTERDREAM_API ADragonController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* myPawnPlayer) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	UPROPERTY()
	class ADragonCharacter* dragonCharacter;

	void MoveToMouseCursor();

	UFUNCTION()
	void CheckMouseCursorShape();

	void SetNewDestination(const FVector destination);
	FHitResult mouseHit;

	bool bclickRightMouse;

	bool bOnMouseCursorRecord;

	UDrangonAnimInstance* dragonInstance;	

	FVector orbFireLoc;
	FVector orbTargetLoc;
	FRotator orbFireRot;
	FHitResult orbHitResult;

	APlayerController* pc;

	//input
	void InputMouseRightPressed();
	void InputMouseRightReleased();
	void InputMouseSpacePressed();
	void InputQPressed();
	void InputWPressed();
	void InputWUnPressed();
	void InputCPressed();

	//widget Cool
	bool bSpaceToggleCooldown = false;
	bool bQToggleCooldown = false;
	bool bWToggleCooldown = false;
};
