// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../../MainCharacter/DragonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

void UDragonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TextBlock_Space->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_Q->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_WSkill->SetVisibility(ESlateVisibility::Hidden);

	UMaterialInterface* materialInstance = Cast<UMaterialInterface>(Image_DragonEnergyOrb->GetBrush().GetResourceObject());
	if (materialInstance)
	{
		dynamicMaterial = UMaterialInstanceDynamic::Create(materialInstance, this);
		if (dynamicMaterial)
		{
			FSlateBrush newBrush = Image_DragonEnergyOrb->GetBrush();
			newBrush.SetResourceObject(dynamicMaterial);
			Image_DragonEnergyOrb->SetBrush(newBrush);

			dynamicMaterial->SetScalarParameterValue(FName("Percentage"), 0.0f);
		}
	}
	//StartPercentageOrbCountDown();
}

void UDragonWidget::updateSpaceCountDown(float coolTime)
{
	if (GlideAnimation)
	{
		PlayAnimation(GlideAnimation);
	}
	currentSpaceCoolTime = coolTime;
	if (currentSpaceCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillSpaceCoolTime, this, &UDragonWidget::updateSpaceCoolTimeText, 0.1f, true);
	}
	else if (currentSpaceCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillSpaceCoolTime);
		currentSpaceCoolTime = 0.0f;

	}
}

void UDragonWidget::updateQCountDown(float coolTime)
{
	if (QAnimation)
	{
		PlayAnimation(QAnimation);
	}
	currentQCoolTime = coolTime;
	if (currentQCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillQCoolTime, this, &UDragonWidget::updateQCoolTimeText, 0.1f, true);
	}
	else if (currentQCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillQCoolTime);
		currentQCoolTime = 0.0f;

	}
}

void UDragonWidget::updateWCountDown(float coolTime)
{
	if (WAnimation)
	{
		PlayAnimation(WAnimation);
	}
	currentWCoolTime = coolTime;
	if (currentWCoolTime > 1.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(th_skillWCoolTime, this, &UDragonWidget::updateWCoolTimeText, 0.1f, true);
	}
	else if (currentWCoolTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_skillWCoolTime);
		currentWCoolTime = 0.0f;

	}
}

void UDragonWidget::updateSpaceCoolTimeText()
{
	if (TextBlock_Space)
	{
		TextBlock_Space->SetVisibility(ESlateVisibility::Visible);
		if (currentSpaceCoolTime > 0.0f)
		{
			currentSpaceCoolTime -= 0.1f;

			if (currentSpaceCoolTime <= 0.9f)
			{
				TextBlock_Space->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentSpaceCoolTime)));
			}
			else
			{
				TextBlock_Space->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentSpaceCoolTime))));
			}

			if (currentSpaceCoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillSpaceCoolTime);
				TextBlock_Space->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UDragonWidget::updateQCoolTimeText()
{
	if (TextBlock_Q)
	{
		TextBlock_Q->SetVisibility(ESlateVisibility::Visible);
		if (currentQCoolTime > 0.0f)
		{
			currentQCoolTime -= 0.1f;

			if (currentQCoolTime <= 0.9f)
			{
				TextBlock_Q->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentQCoolTime)));
			}
			else
			{
				TextBlock_Q->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentQCoolTime))));
			}

			if (currentQCoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillQCoolTime);
				TextBlock_Q->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UDragonWidget::updateWCoolTimeText()
{
	if (TextBlock_WSkill)
	{
		TextBlock_WSkill->SetVisibility(ESlateVisibility::Visible);
		if (currentWCoolTime > 0.0f)
		{
			currentWCoolTime -= 0.1f;

			if (currentWCoolTime <= 0.9f)
			{
				TextBlock_WSkill->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), currentWCoolTime)));
			}
			else
			{
				TextBlock_WSkill->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(currentWCoolTime))));
			}

			if (currentWCoolTime <= 0.0f)
			{
				GetWorld()->GetTimerManager().ClearTimer(th_skillWCoolTime);
				TextBlock_WSkill->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UDragonWidget::SetPercent(float percent)
{
	if (dynamicMaterial)
	{
		float clamped = FMath::Clamp(percent, 0.0f, 1.0f);
		dynamicMaterial->SetScalarParameterValue(FName("Percentage"), clamped);
	}
}

void UDragonWidget::StartPercentageOrbCountDown()
{
	orbElapsedTime = 0.f;
	SetPercent(1.f);
	GetWorld()->GetTimerManager().SetTimer(th_OrbPercentage,this,&UDragonWidget::UpdatePercentageOrb, timeInterval,true);
}

void UDragonWidget::UpdatePercentageOrb()
{
	orbElapsedTime += timeInterval;
	float orbAlpha = FMath::Clamp(orbElapsedTime / duration, 0.0f, 1.0f);
	float orbNewPercentage = FMath::Lerp(1.f, 0.f, orbAlpha);
	SetPercent(orbNewPercentage);
	if (orbAlpha >= 1.0f)
	{
		ADragonCharacter* currentDragon = Cast<ADragonCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		currentDragon->PolyMolfCancle();
		GetWorld()->GetTimerManager().ClearTimer(th_OrbPercentage);
	}
}


