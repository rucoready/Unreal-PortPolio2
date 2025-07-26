// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"



AStartMainMenuGameMode::AStartMainMenuGameMode()
{
	
}

void AStartMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(this, bgm);
	
}
