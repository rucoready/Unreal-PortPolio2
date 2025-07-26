// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "NiagaraSystem.h"
#include "../SpawnActor/GreatSwordActor.h"
#include "../SpawnActor/IndicatorDecalActor.h"
#include "../SpawnActor/AoeCircleDecal.h"
#include "GenericTeamAgentInterface.h"
#include "MainWidget.h"
#include "../MainCharacter/CharacterWidget/BloodWidget.h"
#include "CineCameraComponent.h"
#include "SequenceDragon.h"
#include "../Npc/NpcUits.h"
class ASwordPlayController;
#include "SwordCharacter.generated.h"

UCLASS()
class BLASTERDREAM_API ASwordCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwordCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	class UStaticMeshComponent* basicSwordMesh;

	bool bHasAttackMode;

	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* meleeAttackAnimMontage01;

	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* meleeAttackAnimMontage02;

	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* meleeAttackAnimMontage03;

	FTimerHandle th_ComboReset;

	FTimerHandle th_BasicAttackDelay;

	void resetComboAttack();

	void meleeAttack();

	bool isDuringAttack;

	bool bHasDelayAttack;

	int comboAttackNum;

	void delayAttack();

	void animLookMouse();

	void updateUI();

	void backToOriginMaxWalkSpeed();
	void changeRunMaxWalkSpeed();
	FVector mouseEndLocation;
	FVector mouseEndDirection;

	//Player Status 
	//HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	float playerMaxHp = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float playerCurrentHp;

	//Mp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float playerMaxMp = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float playerCurrentMp;


	//Force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float playerMaxForce = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float playerCurrentForce;


	FTimerHandle th_decreaseForce;

	void decreaseForceAfterUseForce();
	void decreaseForceTick();
	void forceAwakeEnd();

	float forceDecreaseAmount;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	class USwordCharacterWidget* characterWidget;

	

	UPROPERTY(EditAnywhere, Category = "Force")
	UAnimMontage* forceAwakeAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystemComponent* forceAwakeParticle;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* awakeLightningStrike;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* forceAwakeNiagara;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraComponent* forceAwakeLightning;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraComponent* forceAwakeFirstComp;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraComponent* forceBodyLightning;

	UFUNCTION()
	void forceAwakeAfter();

	FTimerHandle th_AwakeForce;
	
	UFUNCTION()
	void playForceAwake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* forceMaterialInstance;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterial* basicSwordMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterial* forceSwordMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> forceCameraShake;

	//force Upgrade Skill
	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* forceMttackAnimMontage01;

	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* forceMttackAnimMontage02;

	UPROPERTY(EditAnywhere, Category = "AttackAnimMontage")
	UAnimMontage* forceMttackAnimMontage03;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* ForceQSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* ForceWSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* ForceESkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* ForceRSkillAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> forceWCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> forceRCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> forceQCameraShake;




	bool bForceMode = false;

	UFUNCTION()
	void cameraShakeR();

	UFUNCTION()
	void cameraShakeQ();

	UFUNCTION()
	void spawnRLastSlash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* forceRLastSkillNia;
	
	//Decal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyDecal")
	TSubclassOf<AIndicatorDecalActor> skillIndicatorDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyDecal")
	TSubclassOf<AAoeCircleDecal> skillAOEDecal;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyDecal")
	TSubclassOf<AIndicatorDecalActor> skillImpactIdicator;

	UFUNCTION()
	void spawnSkillIndicatorDecal();

	UFUNCTION()
	void spawnSkillImpactDecal(float size);

	UFUNCTION()
	void spawnUnderSkillImpactDecal(float size);

	UFUNCTION()
	void spawnSkillIndicatorDecalAVer();


	UFUNCTION()
	void DestroySkillIndicatorDecal();

	UFUNCTION()
	void DestroyImpactIndicatorDecal();

	UFUNCTION()
	void spawnAOEDecal(float spawnSize);

	UFUNCTION()
	void DestroyAOEDecal();

	bool bAOESpawnTime;

	float aoeRadius;
	float currentZ;
	class AIndicatorDecalActor* spawnDecal;
	class AIndicatorDecalActor* spawnImpactDecal;
	class AAoeCircleDecal* spawnAOE;
	class AAoeCircleDecal* spawnAoeR;
	ASwordPlayController* swordPlayerController;

	FVector rTargetLoc;
	FVector chLoc;

	bool bRTooHigh = false;
	bool bInCircle = false;
	void checkInAOEDecalRange();

	FTimerHandle th_ASkillCheckTimer;
	int skillACountSnapshot = -1;

	UFUNCTION()
	void checkSkillSnapshot();

	
	

	


	//skill
	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* qSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* wSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* eSkillAnim;
	
	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* eSkillStartAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimSequence* eSkillEndAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* rSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillStartAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillFallingAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* sSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* dSkillAnim;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* fSkillAnim;



	//A SKILL
	bool bSkillAActive;

	UFUNCTION()
	void StopAOverHeight();

	UFUNCTION()
	void HoveringCheck();

	bool bHovering;
	bool bIsAiming = false;

	UFUNCTION()
	void OnSkillStartAnimEnded(UAnimMontage* montage, bool bInterrupted);

	UFUNCTION()
	void OnSkillEndSkillA(UAnimMontage* montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillAiming;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillEndAttack;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillAimingAttack1;

	UPROPERTY(EditAnywhere, Category = "SKILLS")
	class UAnimMontage* aSkillAimingAttack2;

	UFUNCTION()
	void AAimingSkillCoolTimeSet();

	UFUNCTION()
	void AAimingSkillCoolTimeReSet();

	UFUNCTION()
	void AAimingSkillEndLaunch();

	FTimerHandle th_aAimingAttackCooltime;
	FTimerHandle th_aAimingAttackEnd;
	bool bCanUseNextSkillA = true;
	UAnimInstance* animInstance;

	int32 skillACount = 0;
	const int32 maxSkillACount = 3;

	UPROPERTY(EditAnywhere, Category="Slash")
	TSubclassOf<class AActor>slashActor;

	UFUNCTION()
	void SpawnSwordSlash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> slashActorHitCameraShake;

	UFUNCTION()
	void SlashHitCameraShake();



	//---------------------------------------
	

	UFUNCTION()
	void CheckInAirArea();

	FVector jumpStartLoc;
	bool bJumpStarted = false;
	FVector aSkillStartCurrentLoc;
	FVector currentLoc;

	bool bIsInAir;
	bool bHoldingAKeyboard;
	

	UFUNCTION()
	void playSkillQ();

	UFUNCTION()
	void playSkillW();

	UFUNCTION()
	void playSkillE();

	UFUNCTION()
	void playSkillR();

	UFUNCTION()
	void playSkillA();

	UFUNCTION()
	void playSkillAEnd();

	UFUNCTION()
	void playSkillS();

	UFUNCTION()
	void playSkillD();

	UFUNCTION()
	void playSkillF();

	UFUNCTION()
	void unlockKey();





	bool bCanUseSkillQ = true;
	float skillQCoolTime = 5.0f;
	float currentSkillQTime = 0.0f;

	bool bCanUseSkillW = true;
	float skillWCoolTime = 11.0f;
	float currentSkillWTime = 0.0f;

	bool bCanUseSkillE = true;
	float skillECoolTime = 9.0f;
	float currentSkillETime = 0.0f;

	bool bCanUseSkillR = true;
	float skillRCoolTime = 7.0f;
	float currentSkillRTime = 0.0f;

	bool bCanUseSkillA = true;
	float skillACoolTime = 25.0f;
	float currentSkillATime = 0.0f;

	bool bIsLockKey = false;
	FTimerHandle th_UnlockSkill;


	//camera zoom in and out
	void cameraWZoomIn();
	void cameraWZoomOut();
	void cameraFarAway();
	void cameraSetOrigin();

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float currentFov = 90.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float zoomFov = 85.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float outFov = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float zoomSpeed = 2.0f;

	FTimerHandle th_zoomInTimer;
	FTimerHandle th_zoomOutTimer;
	FTimerHandle th_zoomFarAwayTimer;

	void zoomInStep();
	void zoomOutStep();
	void zoomFarAwayStep();
	


	//ESkill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword")
	TSubclassOf<class AActor> originGreatSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword")
	TSubclassOf<class AActor> forceGreatSword;

	UFUNCTION()
	void spawnOriginGreatSword();

	UFUNCTION()
	void spawnForceGreatSword();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> originEGreatSwordShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> forceEGreatSwordShake;

	UFUNCTION()
	void CameraShakeEGreatSwordNormal();

	UFUNCTION()
	void CameraShakeEGreatSwordForce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword")
	UParticleSystem* eNormalImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GreatSword")
	UParticleSystem* eForceImpactParticle;

	//overlapSword
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySword")
	class UChildActorComponent* overlapSword;

	class ASwordActor* swordActorInstance;

	



	//Data Table
	UFUNCTION()
	void getDamageDatableValue(float damage);

	class UStatGameInstance* statGI;
	
	float normalAttackDamage;
	float normalLastAttackDamage;
	float qAttackDamage;
	float qAttackLastDamage;
	float wAttackDamage;
	float wAttackLastDamage;
	float eAttackDamage;
	float eAttackLastDamage;
	float rAttackDamage;
	float rAttackLastDamage;

	FString skillName;



	UFUNCTION()
	void ClearSkillName();


	//MotionWarping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> motionWarpComponent;

	UFUNCTION()
	void MotionWarpingMouseCursor();

	//ignore
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<AActor>IgnoreKnockbackActor;

	UFUNCTION()
	void SpawnIgnoreKnockBackActor();

	//s skill
	bool bSSkillMoving;
	FVector SSkilltargetLoc;
	float sSkillMoveSpeed = 10.0f;

	UFUNCTION()
	void SkillSMoving(float DeltaTime);

	UFUNCTION()
	void StartSSkillMove();

	UFUNCTION()
	void spawnSkillSSlashNiagara();

	UPROPERTY(EditAnywhere, Category="SKILL")
	UNiagaraSystem* sSlashNiagara;

	UFUNCTION()
	void StartRadialBlur();

	UFUNCTION()
	void EndRadialBlur();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostProcess", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* radialBlurMaterialInstance;

	//ghost trail
	UPROPERTY(EditAnywhere, Category = "GhostTrail")
	UNiagaraComponent* ghostTrailComponent;

	UFUNCTION()
	void ShowGhostTrail();

	UFUNCTION()
	void HideGhostTrail();


	//minimap camera
	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class USpringArmComponent* minimapCameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class USceneCaptureComponent2D* minimapCapture;

	UPROPERTY(VisibleAnywhere, Category = "MinimapCamera")
	class UPaperSpriteComponent* minimapSprite;

	//Isomatric View Camera Moving
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float originArmLength = 800.0f;

	float minArmLength = 500.0f;
	float currentArmLength = 800.0f;
	float armLengthChangeSpeed = 10.0f;
	
	UFUNCTION()
	void UpdateCameraArmLength(float DeltaTime);

	bool bShouldLerpArmLength = false;

	//GetDamage
	UFUNCTION()
	void GetDamage(float damage);

	//id
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(1);
	}

	//crits
	bool bCritical = false;
	int critsPercent = 10;

	//gamemodeui
	UFUNCTION()
	void SetUpdateGameModeGamePlayWidget();

	class ASwordPlayerGameBase* gameMode;

	bool InGameModeUserWidgetSet;

	//sequence
	UPROPERTY(EditAnywhere, Category="Sequence")
	class UAnimMontage* switchSwordMontage;

	UFUNCTION()
	void playSwitchAnim();

	UStatGameInstance* myInstance;

	//revive
	UPROPERTY(EditAnywhere, Category = "Revive")
	class UAnimMontage* dieMontage;

	UPROPERTY(EditAnywhere, Category = "Revive")
	class UAnimMontage* respawnMontage;

	UFUNCTION()
	void TriggerDeathAndRevive(bool bIsDie);

	UFUNCTION()
	void OnDieMontageEnded();


	FTimerHandle th_DieMontageEnd;

	UFUNCTION()
	void ShowViewPortMainWidget();

	UFUNCTION()
	void ShowViewPortMainWidgetGameModeVer();

	//캐릭터 체력 관련(회복)--
	void usingItems(const FString& UsingItemName);

	UPROPERTY(EditAnywhere, Category="Potion")
	USoundBase* usePoitionSound;

	void usingPropital(float DeltaTime);

	bool usingP_potion = false;
	float RegenRateHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;

	//widget setting - dove
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	class UMainWidget* equipmentUI;

	//pause Widget
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPauseWidget> PauseWidgetClass;

	class UPauseWidget* pauseWidget;

	UFUNCTION()
	void SetPauseWidget();

	//foot Step
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_NormalTile;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_DungeonTile;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_Sand;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_Wood;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_Grass;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_Water;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> sound_Stone;

	//footstep effect
	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* normalTileEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* dungeonTileEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* sandEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* woodEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* grassEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* waterEffect;

	UPROPERTY(EditAnywhere, Category = "FootStep")
	UNiagaraSystem* stoneEffect;
	
	UFUNCTION()
	void FootStep();

	bool bPrevForceMode = false;

	//sound skill
	UPROPERTY(EditAnywhere, Category = "SkillSound")
	USoundBase* awakeSound;

	UPROPERTY(EditAnywhere, Category = "SkillSound")
	USoundBase* maxForceChargingSound;

	bool bPlayedMaxForceSound = false;

	UPROPERTY(EditAnywhere, Category = "SkillSound")
	USoundBase* sSkillSound;

	UPROPERTY(EditAnywhere, Category = "SkillSound")
	USoundBase* pauseSound;

	float copyDelayBarPercent;

	//blood overay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBloodWidget> bloodWidgetClass;

	class UBloodWidget* bloodWidget;

	bool bWasLowHp = false;

	UPROPERTY(EditAnywhere, Category="LowHp")
	USoundBase* lowHPVoice;
	

	//polymolp
	UFUNCTION()
	void Polymorph();

	UPROPERTY(EditAnywhere, Category = "polymorph")
	UCameraComponent* skillCamera;

	UPROPERTY(EditAnywhere, Category = "polymorph")
	class ULevelSequence* characterPolyMorphSequence;

	UFUNCTION()
	void OnPolymorphSequenceFinished();

	UPROPERTY(EditAnywhere, Category = "polymorph")
	UAnimMontage* montageBackStep;

	UFUNCTION()
	void PlayerVisible();

	UFUNCTION()
	void PlayerUnvisible();

	UPROPERTY(EditAnywhere, Category = "Polymorph")
	TSubclassOf<ASequenceDragon> sequenceDragonClass;

	class ASequenceDragon* sequenceDragon;

	UPROPERTY(EditAnywhere, Category = "Polymorph")
	TSubclassOf<AActor> sequenceDragonPortalClass;

	class AActor* sequenceDragonPortal;

	FTimerHandle th_SequenceDragonMove;
	FVector dragonStartLocation;
	FVector dragonTargetLocation;
	float dragonMoveElapsedTime = 0.f;
	float dragonMoveDuration = 2.f;

	UPROPERTY(EditAnywhere, Category = "Polymorph")
	FRotator dragonSpawnRotator;

	UFUNCTION()
	void SpawnSequenceDragon();

	FTimerHandle th_SpawnSequenceDragon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polymorph")
	TSubclassOf<class UCameraShakeBase> dragonCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Polymorph")
	TSubclassOf<class ADragonCharacter> dragonCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Polymorph")
	TSubclassOf<class ADragonController> dragonControllerClass;

	UFUNCTION()
	void PolyMolfRetrunPlayer();

	UPROPERTY(EditAnywhere, Category = "polymorph")
	UAnimMontage* montageReturnPlayer;

	UPROPERTY(EditAnywhere, Category = "polymorph")
	UParticleSystem* returnPlayerParticle;

	UFUNCTION()
	void PossessSetting();

	UFUNCTION()
	void RemoveCharacterWidget();

	class ULevelSequencePlayer* swordSequencePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="polymorph")
	class ACameraActor* SkillCameraActor;

	//polymorlf skill sequence

	APlayerController* newPossessController;
	class ADragonCharacter* newDragon;
	bool bIsDragonSpawned = false;
	UPROPERTY()
	APlayerController* rememberedSwordController = nullptr;

	UFUNCTION()
	void ShowAllWidget();

	UFUNCTION()
	void AfterPolyResetPlayer();

	class UStatGameInstance* gameInstance;

	UFUNCTION()
	void SetLevelMoveUiSave();

	class ASkeletonAIController* skelCon;

	UFUNCTION()
	void SpawnPlayerPortal();

	UFUNCTION()
	void DestroyPlayerPortal();

	UPROPERTY(EditAnywhere, Category="Poly")
	TSubclassOf <AActor> playerPortal;

	UPROPERTY()
	AActor* polySpawnPlayerPortal = nullptr;

	UPROPERTY()
	bool bGInteraction = false;

	UPROPERTY()
	ANpcUits* currentInteractionNpc = nullptr;

};
