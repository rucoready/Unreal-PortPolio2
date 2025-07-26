// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Components/Image.h"
#include "MediaSoundComponent.h"
#include "IntroWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERDREAM_API UIntroWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_IntroVideo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	UMediaPlayer* introPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	UMediaSource* introMediaSource;

	UPROPERTY()
	UMediaSoundComponent* mediaSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString targetLoadingMap;

	UFUNCTION()
	void IntroFinished();

	
};
