// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_potion.h"

AMP_potion::AMP_potion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>mpMesh(TEXT("/Script/Engine.StaticMesh'/Game/DoveDove/potions/source/Potion_Potion02.Potion_Potion02'"));
	if (mpMesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(mpMesh.Object);
	}
	ItemName = FString(TEXT("MP_potion"));

}
