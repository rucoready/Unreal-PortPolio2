// Fill out your copyright notice in the Description page of Project Settings.


#include "HP_potion.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager/StatGameInstance.h"



AHP_potion::AHP_potion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>hpMesh(TEXT("/Script/Engine.StaticMesh'/Game/DoveDove/potions/source/Potion_Potion01.Potion_Potion01'"));
	if (hpMesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(hpMesh.Object);
	}

	ItemName = FString(TEXT("HP_potion"));

}

