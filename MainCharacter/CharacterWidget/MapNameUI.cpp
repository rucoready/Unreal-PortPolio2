// Fill out your copyright notice in the Description page of Project Settings.


#include "MapNameUI.h"
#include "Components/TextBlock.h"

void UMapNameUI::playMapNameWidget()
{
	TextBlock_MapName->SetText(FText::FromString(mapName));
	PlayAnimation(MapNameAnim);
}
