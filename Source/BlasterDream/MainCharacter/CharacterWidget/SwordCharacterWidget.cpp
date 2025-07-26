// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../MainCharacter/SwordPlayController.h"
#include "Components/WidgetComponent.h"


void USwordCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_SkillQ->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_SkillW->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_SkillE->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_SkillR->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_SkillA->SetVisibility(ESlateVisibility::Hidden);
	ProgressBar_ASkill->SetVisibility(ESlateVisibility::Hidden);
	CanvasPanel_Forcing->SetVisibility(ESlateVisibility::Hidden);

	collectionInstance = GetWorld()->GetParameterCollectionInstance(innerSCollection);
	collectionInstanceOutter = GetWorld()->GetParameterCollectionInstance(outterSCollection);
	collectionInstance->SetScalarParameterValue(FName("Progress"), 0.0);
	collectionInstanceOutter->SetScalarParameterValue(FName("Progress"), 0.0);
	soulCollectionInstance = GetWorld()->GetParameterCollectionInstance(soulParameterCollection);
	soulCollectionInstance->SetScalarParameterValue(FName("Progress"), 0.0);


	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(th_DelayHPBar, this, &USwordCharacterWidget::UpdateDelayBar, 0.016f, true);
	}
	if (IsValid(ProgressBar_Delay))
	{
		//ProgressBar_Delay->SetPercent(1.0f);
	}
	targetDelayBarPercent = 1.0f;
}
void USwordCharacterWidget::SetGaze14Gaze()
{
	PlayAnimation(SkillCActivateAnim);
}
void USwordCharacterWidget::updateHpBar(float CurrentHp, float MaxHp)
{
	if (ProgressBar_Hp)
	{
		float barPercent = FMath::Clamp(CurrentHp / MaxHp, 0.0f, 1.0f);
		ProgressBar_Hp->SetPercent(barPercent);
		targetDelayBarPercent = barPercent;
	}
	
}

void USwordCharacterWidget::updateMpBar(float CurrentHp, float MaxHp)
{
	if (ProgressBar_Mp)
	{
		ProgressBar_Mp->SetPercent(FMath::Clamp(CurrentHp / MaxHp, 0.0f, 1.0f));
	}
}

void USwordCharacterWidget::updateForceBar(float CurrentHp, float MaxHp)
{
	if (ProgressBar_Force)
	{
		ProgressBar_Force->SetPercent(FMath::Clamp(CurrentHp / MaxHp, 0.0f, 1.0f));
	}
}
void USwordCharacterWidget::updateASkillBar(float CurrentTime, float MaxTime)
{
	if (ProgressBar_ASkill)
	{
		ProgressBar_ASkill->SetPercent(FMath::Clamp(CurrentTime / MaxTime, 0.0f, 1.0f));
	}
}
void USwordCharacterWidget::UpdateSInnerCountDown(float coolTime)
{
	if (innerSCollection)
	{
		if (collectionInstance)
		{
			currentSCoolTime = coolTime;
			currentProgress = 0.0f; 


			if (innerProgressBar)  
			{
				
				innerProgressBar->SetVisibility(ESlateVisibility::Visible);  
				collectionInstance->SetScalarParameterValue(FName("Progress"), currentProgress);  
			}

			
			//GetWorld()->GetTimerManager().ClearTimer(th_InnerProgressTimer);
			GetWorld()->GetTimerManager().SetTimer(th_InnerProgressTimer, this, &USwordCharacterWidget::StartSInnerProgressBar, 0.01f, true);
			

			//GetWorld()->GetTimerManager().ClearTimer(th_InnerProgressTimer);
			//GetWorld()->GetTimerManager().SetTimer(th_InnerProgressTimer,this,&USwordCharacterWidget::StartSInnerProgressBar,0.01f,true);
		}
	}
}
void USwordCharacterWidget::StartSInnerProgressBar()
{
	if (collectionInstance)
	{
		//currentProgress = 0.f;
		float deltaProgress = 1.0f / currentSCoolTime * 0.01f; 
		currentProgress += deltaProgress;
		currentProgress = FMath::Clamp(currentProgress, 0.0f, 1.0f); 
		collectionInstance->SetScalarParameterValue(FName("Progress"), currentProgress);
		if (currentProgress >= 1.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(th_InnerProgressTimer);
			innerStack++;
			UpdateSOutterCountDown();

			if (innerStack >= 6)
			{
				if (collectionInstanceOutter)
				{
					innerProgressBar->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			else
			{
				UpdateSInnerCountDown(currentSCoolTime);
			}
		}
	}
}
void USwordCharacterWidget::UpdateSOutterCountDown()
{
	if (collectionInstanceOutter)
	{
		if (innerStack >= 0 && innerStack <= 6)
		{
			float progressIncrement = 1.0f / 6.0f;  // 6등분
			float currentOutterProgress = 0.0f;
			currentOutterProgress = innerStack * progressIncrement;
			currentOutterProgress = FMath::Clamp(currentOutterProgress, 0.0f, 1.0f);
			collectionInstanceOutter->SetScalarParameterValue(FName("Progress"), currentOutterProgress);
		}
	}
	
}

void USwordCharacterWidget::UpdateSOutterDownStack()
{
	if (!IsValid(this))
	{
		return;
	}
	if (innerStack > 0)
	{
		innerStack--;  
		UpdateSOutterCountDown();
		

		if (!GetWorld()->GetTimerManager().IsTimerActive(th_InnerProgressTimer))
		{
			UpdateSInnerCountDown(currentSCoolTime);  
			
		}
	}	
}

void USwordCharacterWidget::SetForceWidgetNiagara()
{
	if (CanvasPanel_Forcing)
	{
		TextBlock_Ctrl->SetVisibility(ESlateVisibility::Visible);
		CanvasPanel_Forcing->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(swordForceReadyAnim);
		PlayAnimation(ForceAnim, 0.0f, 10);	
	}
}

void USwordCharacterWidget::HideForceWidget()
{
	if (CanvasPanel_Forcing)
	{
		CanvasPanel_Forcing->SetVisibility(ESlateVisibility::Hidden);
		StopAnimation(ForceAnim);
	}
}

void USwordCharacterWidget::OnFinishCtrlAnim()
{
	TextBlock_Ctrl->SetVisibility(ESlateVisibility::Hidden);
}

void USwordCharacterWidget::StartForce()
{
	PlayAnimation(SwordForceStart);

}

void USwordCharacterWidget::StopForce()
{
	if (!SwordForceStart)
	{
		return;
	}
	StopAnimation(SwordForceStart);
	PlayAnimation(SwordForceEnd);
}

void USwordCharacterWidget::UpdateDelayBar()
{
	float currentDelayPercent = ProgressBar_Delay->GetPercent();
	float currentHPBarPercent = ProgressBar_Hp->GetPercent();
	if (currentDelayPercent > targetDelayBarPercent)
	{
		float newLerpPercent = FMath::FInterpTo(currentDelayPercent, targetDelayBarPercent, 0.016f, 2.0f);
		ProgressBar_Delay->SetPercent(newLerpPercent);
	}
	else
	{
		ProgressBar_Delay->SetPercent(currentHPBarPercent);
	}
}

void USwordCharacterWidget::ResetTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(th_InnerProgressTimer);
	}
}

FVector2D USwordCharacterWidget::GetSoulHoleScreentPosition()
{
	if (!Image_SoulHole) return FVector2D::ZeroVector;
	// center vector
	FGeometry holeGeometry = Image_SoulHole->GetCachedGeometry();
	FVector2D holeLocalCenter = holeGeometry.GetLocalSize() * 0.5f;

	//Local → Absolute 
	FVector2D AbsolutePosition = holeGeometry.LocalToAbsolute(holeLocalCenter);

	if (UWidget* holeRootWidget = GetRootWidget())
	{
		FGeometry rootGeometry = holeRootWidget->GetCachedGeometry();
		FVector2D rootAbsolutePosition = rootGeometry.GetAbsolutePosition();

		// 절대좌표
		FVector2D screenPosition = AbsolutePosition - rootAbsolutePosition;
		return screenPosition;
	}
	return FVector2D::ZeroVector;
}

void USwordCharacterWidget::SoulCollection()
{
	PlayAnimation(SkillCWidgetAnim);
}

void USwordCharacterWidget::SetSoulCollectionBar(float gaze)
{
	if (!soulCollectionInstance)
	{
		soulCollectionInstance = GetWorld()->GetParameterCollectionInstance(soulParameterCollection);
		if (!soulCollectionInstance) return;
	}
	float totalSteps = 14;
	float clampedGaze = FMath::Clamp(gaze, 0, totalSteps); // 0 ~ 14
	soulProgress = clampedGaze / 14.f;
	soulCollectionInstance->SetScalarParameterValue(FName("Progress"), soulProgress);
}

void USwordCharacterWidget::IncreaseSoulCollectionBar()
{
	currentSoulGaze = FMath::Clamp(currentSoulGaze + 0.5, 0, 14);
	SetSoulCollectionBar(currentSoulGaze);
	APlayerController* nowCon = GetOwningPlayer();
	ASwordPlayController* swordPlayerCon = Cast<ASwordPlayController>(nowCon);
	if (!swordPlayerCon) return;
	if (currentSoulGaze == 14.f && !bGaze14)
	{
		if (swordPlayerCon)
		{
			swordPlayerCon->bCanPoly = true;
			PlayAnimation(SkillCActivateAnim);
			bGaze14 = true;
		}
	}
}

void USwordCharacterWidget::ResetGaze()
{
	currentSoulGaze = 0;
	bGaze14 = false;
	PlayAnimation(SkillCDeactiveAnim);
	soulCollectionInstance->SetScalarParameterValue(FName("Progress"), 0);

}

void USwordCharacterWidget::PlayAnimEndPlySwordReset()
{
	if (!SwordForceStart)
	{
		return;
	}
	StopAnimation(SwordForceStart);
	StopAnimation(swordForceReadyAnim);
	PlayAnimation(SwordForceEndPoly);
}


//skills Q
void USwordCharacterWidget::updateQCountDown(float coolTime)
{
	if (skillQWidgetAnim)
	{
		PlayAnimation(skillQWidgetAnim);
	}
	currentQCoolTime = coolTime;
	if (currentQCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillQCoolTime, this, &USwordCharacterWidget::updateQCoolTimeText, 0.1f, true);
	}
	else if (currentQCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillQCoolTime);
		currentQCoolTime = 0.0f;
		
	}
	
}

void USwordCharacterWidget::updateQCoolTimeText()
{
	if (TextBlock_SkillQ)
	{
		TextBlock_SkillQ->SetVisibility(ESlateVisibility::Visible);
		if (TextBlock_SkillQ)
		{
			if (currentQCoolTime > 0.0f)
			{
				currentQCoolTime -= 0.1f;
				if (currentQCoolTime <= 0.9f)
				{
					TextBlock_SkillQ->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentQCoolTime)));
				}
				else
				{
					TextBlock_SkillQ->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentQCoolTime))));
				}
				if (currentQCoolTime <= 0.0f)
				{
					GetWorld()->GetTimerManager().ClearTimer(th_skillQCoolTime);
					TextBlock_SkillQ->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
	
}
//SKILLW
void USwordCharacterWidget::updateWCountDown(float coolTime)
{
	if (skillWWidgetAnim)
	{
		PlayAnimation(skillWWidgetAnim);
	}
	currentWCoolTime = coolTime;
	if (currentWCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillWCoolTime, this, &USwordCharacterWidget::updateWCoolTimeText, 0.1f, true);
	}
	else if (currentWCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillWCoolTime);
		currentWCoolTime = 0.0f;

	}

}

void USwordCharacterWidget::updateWCoolTimeText()
{
	if (TextBlock_SkillW)
	{
		TextBlock_SkillW->SetVisibility(ESlateVisibility::Visible);
		if (currentWCoolTime > 0.0f)
		{
			currentWCoolTime -= 0.1f;

			if (currentWCoolTime <= 0.9f)
			{
				TextBlock_SkillW->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentWCoolTime)));
			}
			else
			{
				TextBlock_SkillW->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentWCoolTime))));
			}

			if (currentWCoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillWCoolTime);
				TextBlock_SkillW->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

//skillE
void USwordCharacterWidget::updateECountDown(float coolTime)
{
	if (skillEWidgetAnim)
	{
		PlayAnimation(skillEWidgetAnim);
	}
	currentECoolTime = coolTime;
	if (currentECoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillECoolTime, this, &USwordCharacterWidget::updateECoolTimeText, 0.1f, true);
	}
	else if (currentECoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillECoolTime);
		currentECoolTime = 0.0f;

	}

}

void USwordCharacterWidget::updateECoolTimeText()
{
	
	if (TextBlock_SkillE)
	{
		TextBlock_SkillE->SetVisibility(ESlateVisibility::Visible);
		if (currentECoolTime > 0.0f)
		{
			currentECoolTime -= 0.1f;

			if (currentECoolTime <= 0.9f)
			{
				TextBlock_SkillE->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentECoolTime)));
			}
			else
			{
				TextBlock_SkillE->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentECoolTime))));
			}

			if (currentECoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillECoolTime);
				TextBlock_SkillE->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
void USwordCharacterWidget::updateACountDown(float coolTime)
{
	if (IsValid(this) && skillAWidgetAnim)
	{
		PlayAnimation(skillAWidgetAnim);
		currentACoolTime = coolTime;
	}
	
	if (IsValid(this) && currentACoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillACoolTime, this, &USwordCharacterWidget::updateACoolTimeText, 0.1f, true);
	}
	else if (IsValid(this) && currentACoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillACoolTime);
		currentACoolTime = 0.0f;

	}
}
void USwordCharacterWidget::updateACoolTimeText()
{
	
	if (TextBlock_SkillA)
	{
		TextBlock_SkillA->SetVisibility(ESlateVisibility::Visible);
		if (currentACoolTime > 0.0f)
		{
			currentACoolTime -= 0.1f;

			if (currentACoolTime <= 0.9f)
			{
				TextBlock_SkillA->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentACoolTime)));
			}
			else
			{
				TextBlock_SkillA->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentACoolTime))));
			}

			if (currentACoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillACoolTime);
				TextBlock_SkillA->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
//SKILLR
void USwordCharacterWidget::updateRCountDown(float coolTime)
{
	if (skillRWidgetAnim)
	{
		PlayAnimation(skillRWidgetAnim);

	}
	currentRCoolTime = coolTime;
	if (currentRCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillRCoolTime, this, &USwordCharacterWidget::updateRCoolTimeText, 0.1f, true);
	}
	else if (currentRCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillRCoolTime);
		currentRCoolTime = 0.0f;

	}

}

void USwordCharacterWidget::updateRCoolTimeText()
{
	
	if (TextBlock_SkillR)
	{
		TextBlock_SkillR->SetVisibility(ESlateVisibility::Visible);
		if (currentRCoolTime > 0.0f)
		{
			currentRCoolTime -= 0.1f;

			if (currentRCoolTime <= 0.9f)
			{
				TextBlock_SkillR->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentRCoolTime)));
			}
			else
			{
				TextBlock_SkillR->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentRCoolTime))));
			}

			if (currentRCoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillRCoolTime);
				TextBlock_SkillR->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void USwordCharacterWidget::updateAProgressBar()
{
	if (ProgressBar_ASkill)
	{
		currentTimeA -= 0.02f;
		float percent = FMath::Clamp(currentTimeA / maxTime, 0.0f, 1.0f);

		ProgressBar_ASkill->SetPercent(percent);

		if (currentTimeA <= 0.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(th_AProgressBarTimer);
		}
	}
}

void USwordCharacterWidget::StartProgressBarCoolDown(float maxTimeA)
{
	if (ProgressBar_ASkill)
	{
		currentTimeA = maxTimeA;
		GetWorld()->GetTimerManager().SetTimer(th_AProgressBarTimer, this, &USwordCharacterWidget::updateAProgressBar, 0.02f, true);

	}
}

void USwordCharacterWidget::LockDownASkillActive()
{
	PlayAnimation(SkillALockDownAnim);
}

void USwordCharacterWidget::LockDownASkillNonActive()
{
	PlayAnimation(SkillALockDownAnim2);
}
