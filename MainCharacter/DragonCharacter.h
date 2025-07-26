// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../SpawnActor/DragonWOrb.h"
#include "GenericTeamAgentInterface.h"
#include "DragonCharacter.generated.h"

UCLASS()
class BLASTERDREAM_API ADragonCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADragonCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* cameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* springArmComponent;

	//flying
	UFUNCTION()
	void ToggleFlying();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flight)
	bool bFlying = false;

	class UDrangonAnimInstance* dragonAnimInstance;

	UPROPERTY(EditAnywhere, Category = "Flight")
	float flyToggleCoolDown = 5.f;

	UPROPERTY(EditAnywhere, Category = "Flight")
	UAnimMontage* takeOffMontage;

	UPROPERTY(EditAnywhere, Category = "Flight")
	UAnimMontage* takeOnMontage;

	bool bToggleFly = true;

	FTimerHandle th_ToggleFly;

	UAnimInstance* animInstance;

	FOnMontageEnded flyMontageDelegate;

	

	UFUNCTION()
	void OnTakeOff();

	UFUNCTION()
	void OnTakeOn();

	UFUNCTION()
	void DragonFlying();

	UFUNCTION()
	void DragonStopFlying();

	UFUNCTION()
	void DragonQAttack();

	UFUNCTION()
	void DragonWAttack();

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* qAnim;

	float targetFOV = 90.f;       
	float currentFOV = 90.f;       
	float fovInterpSpeed = 2.5f;

	FVector startLoc;
	FVector flyingTargetLoc;
	bool bInterpZ = true;
	float zInterpTime = 0.5f; 
	float zInterpElapsed = 0.f;

	UFUNCTION()
	void MoveToInAir(const FVector& target);

	bool bFlyingMoveToTarget = false;
	FVector flyingTargetLocation;

	FRotator flyingTargetRotation;
	float flyingRotateInterpSpeed = 3.f;

	//widget
	UFUNCTION()
	void ShowViewPortMainWidget();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UDragonWidget> dragonWidgetClass;

	class UDragonWidget* dragonWidget;


	class ASwordPlayerGameBase* gameMode;

	//minimap
	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class USpringArmComponent* minimapCameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class USceneCaptureComponent2D* minimapCapture;

	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class UPaperSpriteComponent* minimapSprite;

	//bs
	

	void UpdateLookAngleToMouse();

	void UpdateCancleLookAngleToMouse();

	//getter function
	float GetLookAngle() const;

	float lookAngle;
	UPROPERTY()
	bool bWAminingNow;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ADragonWOrb>dragonOrbClass;

	class ADragonWOrb* dragonOrb;

	UFUNCTION()
	void WOrbHitCameraShake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> orbHitCameraShake;

	FHitResult orbHitResult;

	//id
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(1);
	}
	UCameraComponent* GetCameraComponent() const { return cameraComponent; }

	//hold camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* polyMolfHoldCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	AActor* polyMolfHoldCameraActor;

	UFUNCTION()
	void DragonVisible();

	UFUNCTION()
	void DragonUnVisible();

	UFUNCTION()
	void PolyMolfCancle();

	UFUNCTION()
	void PolyMolfCancleAnimationFinished();

	UPROPERTY(EditAnywhere, Category = "Poly")
	UAnimMontage* polymolfCancleMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Polymorph")
	TSubclassOf<class ASwordCharacter> swordCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Polymorph")
	TSubclassOf<class ASwordPlayController> swordCharacterControllerClass;

	class ADragonController* dragonController;

	UFUNCTION()
	void PossessSetting();

	UFUNCTION()
	void RemoveDragonWidget();

	UPROPERTY()
	ASwordCharacter* rememberedSwordCharacter;

	UPROPERTY(EditAnywhere, Category="Scene")
	USoundBase* dragonOrbChargeSound;

};

