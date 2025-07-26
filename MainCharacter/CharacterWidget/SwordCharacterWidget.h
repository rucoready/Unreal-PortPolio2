// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraComponent.h"
#include "SwordCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API USwordCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Hp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Mp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Force;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_ASkill;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Delay;
	
	UFUNCTION()
	void SetGaze14Gaze();

	//skill
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SkillQ;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SkillW;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SkillE;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SkillR;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SkillA;

	//minimapSetting
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Skill;

	

	//SKILLQ
	UFUNCTION()
	void updateQCountDown(float coolTime);

	UFUNCTION()
	void updateQCoolTimeText();
	//SKILLW
	UFUNCTION()
	void updateWCountDown(float coolTime);

	UFUNCTION()
	void updateWCoolTimeText();
	//SKILLE
	UFUNCTION()
	void updateECountDown(float coolTime);

	UFUNCTION()
	void updateECoolTimeText();

	//SKILLA
	UFUNCTION()
	void updateACountDown(float coolTime);

	UFUNCTION()
	void updateACoolTimeText();

	//SKILLR
	UFUNCTION()
	void updateRCountDown(float coolTime);

	UFUNCTION()
	void updateRCoolTimeText();

	//SkillA
	UFUNCTION()
	void updateAProgressBar();

	FTimerHandle th_AProgressBarTimer;
	float currentTimeA = 6.0f;
	float maxTime = 6.0f;

	UFUNCTION()
	void StartProgressBarCoolDown(float maxTimeA);

	UPROPERTY(meta = (BindWidgetAnim), Transient) 
	class UWidgetAnimation* skillQWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* skillWWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* skillEWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* skillRWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* skillAWidgetAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillALockDownAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillALockDownAnim2;

	

	

	UFUNCTION()
	void LockDownASkillActive();

	UFUNCTION()
	void LockDownASkillNonActive();

	FTimerHandle th_skillQCoolTime;
	float currentQCoolTimeDisplay = 0.0f;
	float currentQCoolTime;

	FTimerHandle th_skillWCoolTime;
	float currentWCoolTimeDisplay = 0.0f;
	float currentWCoolTime;

	FTimerHandle th_skillECoolTime;
	float currentECoolTimeDisplay = 0.0f;
	float currentECoolTime;

	FTimerHandle th_skillRCoolTime;
	float currentRCoolTimeDisplay = 0.0f;
	float currentRCoolTime;

	FTimerHandle th_skillACoolTime;
	float currentACoolTimeDisplay = 0.0f;
	float currentACoolTime;
	

	void updateHpBar(float CurrentHp, float MaxHp);
	void updateMpBar(float CurrentMp, float MaxMp);
	void updateForceBar(float CurrentForce, float MaxForce);
	void updateASkillBar(float CurrentTime, float MaxTime);

	//skillS MPC
	UPROPERTY(EditAnywhere, Category="MPC")
	UMaterialParameterCollection* innerSCollection;

	UPROPERTY(EditAnywhere, Category = "MPC")
	UMaterialParameterCollection* outterSCollection;

	UFUNCTION()
	void UpdateSInnerCountDown(float coolTime);

	UFUNCTION()
	void StartSInnerProgressBar();

	FTimerHandle th_InnerProgressTimer;
	float currentProgress = 0.0f;
	float currentSCoolTime = 0.0f;
	UMaterialParameterCollectionInstance* collectionInstance;

	UMaterialParameterCollectionInstance* collectionInstanceOutter;

	UFUNCTION()
	void UpdateSOutterCountDown();

	UFUNCTION()
	void UpdateSOutterDownStack();


	int innerStack = 0;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* innerProgressBar;

	//widget niagara
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_Forcing;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Ctrl;

	UFUNCTION()
	void SetForceWidgetNiagara();

	UFUNCTION()
	void HideForceWidget();

	UFUNCTION()
	void OnFinishCtrlAnim();

	UFUNCTION()
	void StartForce();

	UFUNCTION()
	void StopForce();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* ForceAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* swordForceReadyAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SwordForceStart;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SwordForceEnd;

	//delay progressbar
	FTimerHandle th_DelayHPBar;
	float targetDelayBarPercent = 1.0;

	UFUNCTION()
	void UpdateDelayBar();

	UFUNCTION()
	void ResetTimer();

	//2d soul widget
	UFUNCTION()
	FVector2D GetSoulHoleScreentPosition();

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_SoulHole;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillCWidgetAnim;

	UFUNCTION()
	void SoulCollection();

	UPROPERTY(EditAnywhere, Category = "MPC")
	UMaterialParameterCollection* soulParameterCollection;
	
	UMaterialParameterCollectionInstance* soulCollectionInstance;

	UFUNCTION()
	void SetSoulCollectionBar(float gaze);

	float soulProgress;

	UFUNCTION()
	void IncreaseSoulCollectionBar();
	
	float currentSoulGaze = 0;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillCActivateAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillCDeactiveAnim;

	UFUNCTION()
	void ResetGaze();

	UPROPERTY()
	bool bGaze14 = false;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SwordForceEndPoly;
	UFUNCTION()
	void PlayAnimEndPlySwordReset();


};
