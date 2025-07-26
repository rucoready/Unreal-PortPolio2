// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonHPBarWidget.h"
#include "Components/ProgressBar.h"


void USkeletonHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(th_DelayBar, this, &USkeletonHPBarWidget::UpdateDelayBar, 0.016f, true);
    }
    if (IsValid(pb_healthBar_DelayBar))
    {
        pb_healthBar_DelayBar->SetPercent(1.0f); 
    }
    targetDelayBarPercent = 1.0f;
}


void USkeletonHPBarWidget::updateHpBar(float currentHp, float maxHp)
{
	if (maxHp <= 0.0f)
	{
		return;
	}
	currentHp = FMath::Max(currentHp, 0.0f);
	if (IsValid(pb_healthBar))
	{
        float barPercent = FMath::Clamp(currentHp / maxHp, 0.0f, 1.0f);
        pb_healthBar->SetPercent(barPercent);

        FLinearColor barColor;
        if (barPercent >= 0.7f)
        {
            //ÁøÇÏ°í »¡°­
            barColor = FLinearColor(0.6f, 0.0f, 0.0f);
        }
        else if (barPercent >= 0.4f)
        {
            //ÁÖÈ²
            barColor = FLinearColor(1.0f, 0.5f, 0.0f);
        }
        else
        {
            //¹àÀº »¡°­
            barColor = FLinearColor(1.0f, 0.05f, 0.05f);
        }

        pb_healthBar->SetFillColorAndOpacity(barColor);
        targetDelayBarPercent = barPercent;
	} 
    if (currentHp <= 0.0f)
    {
        pb_healthBar_DelayBar->SetVisibility(ESlateVisibility::Collapsed);
        pb_healthBar->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void USkeletonHPBarWidget::UpdateDelayBar()
{
    currentDelayPercent = pb_healthBar_DelayBar->GetPercent();
    if (currentDelayPercent > targetDelayBarPercent)
    {
        float newLerpPercent = FMath::FInterpTo(currentDelayPercent, targetDelayBarPercent, 0.016f, 2.0f);
        pb_healthBar_DelayBar->SetPercent(newLerpPercent);
    }
    else if (currentDelayPercent < targetDelayBarPercent)
    {
        float newLerpPercent = FMath::FInterpTo(currentDelayPercent, targetDelayBarPercent, 0.016f, 4.0f);
        pb_healthBar_DelayBar->SetPercent(newLerpPercent);
    }
    else
    {
        pb_healthBar_DelayBar->SetPercent(targetDelayBarPercent);
    }
}




