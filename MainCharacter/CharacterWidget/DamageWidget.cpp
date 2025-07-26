// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideWidget();
}

void UDamageWidget::SetDamageText(float damage)
{
	damage = 88.0f;
	TextBlock_Damage->SetText(FText::AsNumber(damage));
}

void UDamageWidget::HideWidget()
{
	TextBlock_Damage->SetVisibility(ESlateVisibility::Collapsed);
}

void UDamageWidget::PlayNormalDamageAnimation(float damage)
{
	PlayAnimation(damageAnimNormalDamage);
	TextBlock_Damage->SetText(FText::AsNumber(damage));
}

void UDamageWidget::PlayCriticalDamageAnimation(float damage)
{
	PlayAnimation(damageAnimCriticalDamage);
	TextBlock_Damage->SetText(FText::AsNumber(damage));
}
