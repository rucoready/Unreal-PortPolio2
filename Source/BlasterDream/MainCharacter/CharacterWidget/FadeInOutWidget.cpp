// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeInOutWidget.h"
#include "Components/Border.h"

void UFadeInOutWidget::NativeConstruct()
{

}

void UFadeInOutWidget::PlayFadeIn()
{
	PlayAnimation(FadeIn);
}

void UFadeInOutWidget::PlayFadeOut()
{
	PlayAnimation(FadeOut);
}
