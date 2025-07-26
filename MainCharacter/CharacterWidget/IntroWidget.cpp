// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroWidget.h"
#include "Kismet/GameplayStatics.h"

void UIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();
	introPlayer->OpenSource(introMediaSource);
	introPlayer->Play();
	introPlayer->OnEndReached.AddDynamic(this, &UIntroWidget::IntroFinished);
}

void UIntroWidget::IntroFinished()
{
	if (!targetLoadingMap.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, FName(*targetLoadingMap));
	}
}
