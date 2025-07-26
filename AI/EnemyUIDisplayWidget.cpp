// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUIDisplayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyUIDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyUIDisplayWidget::updateHpBar(FString name, float currentHp, float maxHp)
{
	if (maxHp <= 0.0f)
	{
		return;
	}
	currentHp = FMath::Max(currentHp, 0.0f);
	if (IsValid(ProgressBar_HPBar))
	{
		ProgressBar_HPBar->SetPercent(FMath::Clamp(currentHp / maxHp, 0.0f, 1.0f));
	}
	if (IsValid(TextBlock_Name))
	{
		FText nameText = FText::FromString(name);  
		TextBlock_Name->SetText(nameText);
	}
	if (IsValid(TextBlock_HPDisplay))
	{
		FString hpText = FString::Printf(TEXT("%.0f / %.0f"), currentHp, maxHp);  
		FText hpDisplayText = FText::FromString(hpText);
		TextBlock_HPDisplay->SetText(hpDisplayText);
	}
}
