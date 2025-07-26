// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodWidget.h"

void UBloodWidget::PlayBloodAnimation()
{
    if (IsAnimationPlaying(BloodAnim))
    {
        StopAnimation(BloodAnim);
    }
    else
    {
        PlayAnimation(BloodAnim, 0.f, 0); 
    }
}
