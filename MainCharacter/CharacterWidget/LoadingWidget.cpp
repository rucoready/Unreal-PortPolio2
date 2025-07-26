// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget.h"
#include "Components/ProgressBar.h"
#include "../../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Components/Image.h"
#include "MediaTexture.h"
#include "Components/TextBlock.h"

void ULoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UGameInstance* baseInstance = UGameplayStatics::GetGameInstance(this);
    UStatGameInstance* gameInstance = Cast<UStatGameInstance>(baseInstance);
    if (gameInstance)
    {
        FName targetLevelName = FName(*gameInstance->lastTargetLevel);
        FLatentActionInfo latentInfo;
        latentInfo.CallbackTarget = this;
        latentInfo.ExecutionFunction = FName("OnLevelLoaded");
        latentInfo.Linkage = 0;
        latentInfo.UUID = __LINE__;

        UGameplayStatics::LoadStreamLevel(this, targetLevelName, true, false, latentInfo);

        // 미디어 텍스처 연결
        if (gameInstance->katanaMediaTexture && Image_LoadingKatana)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(gameInstance->katanaMediaTexture);
            Brush.ImageSize = FVector2D(1920, 1080);
            Image_LoadingKatana->SetBrush(Brush);
        }

        // 이미 OpenSource가 끝났고 준비되었으니 → Play만 함
        if (gameInstance->katanaMediaPlayer && gameInstance->katanaMediaPlayer->IsReady())
        {
            gameInstance->katanaMediaPlayer->Play();
        }
        
    }
   
    //katanaLoadingPlayer->OpenSource(katanaMediaPlayer);
    //katanaLoadingPlayer->Play();
}


void ULoadingWidget::LoadingStart()
{
    ProgressBar_Loading->SetPercent(0.0f);
    elapsedTime = 0.0f;
    totalTime = 4.0f;

    GetWorld()->GetTimerManager().SetTimer(th_Progressbar, this, &ULoadingWidget::UpdateProgressBar, 0.01f, true);

}

void ULoadingWidget::UpdateProgressBar()
{
    elapsedTime += 0.01f;
	float alpha = elapsedTime / totalTime;
	alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
	ProgressBar_Loading->SetPercent(alpha);
	if (alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(th_Progressbar);

		UStatGameInstance* gameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (gameInstance)
		{
            UGameplayStatics::OpenLevel(this, FName(*gameInstance->lastTargetLevel));
            
		}
	}
}

void ULoadingWidget::StartDotAnim()
{
    if (!GetWorld()) return;

    TWeakObjectPtr<ULoadingWidget> weakLoadingPointer(this);
    int32 dotIndex = 0;

    GetWorld()->GetTimerManager().SetTimer(th_DotAnimation, [weakLoadingPointer, dotIndex]() mutable
    {
        if (!weakLoadingPointer.IsValid() || !weakLoadingPointer->IsInViewport())
        {
            if (weakLoadingPointer.IsValid() && weakLoadingPointer->GetWorld())
                weakLoadingPointer->GetWorld()->GetTimerManager().ClearTimer(weakLoadingPointer->th_DotAnimation);
            return;
        }

        switch (dotIndex)
        {
        case 0:
            if (weakLoadingPointer->Dot1Animation) weakLoadingPointer->PlayAnimation(weakLoadingPointer->Dot1Animation);
            break;
        case 1:
            if (weakLoadingPointer->Dot2Animation) weakLoadingPointer->PlayAnimation(weakLoadingPointer->Dot2Animation);
            break;
        case 2:
            if (weakLoadingPointer->Dot3Animation) weakLoadingPointer->PlayAnimation(weakLoadingPointer->Dot3Animation);
            break;
        }

        dotIndex = (dotIndex + 1) % 3;

    }, 0.1f, true);
}





