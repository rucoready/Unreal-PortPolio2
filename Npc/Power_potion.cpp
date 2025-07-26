// Fill out your copyright notice in the Description page of Project Settings.


#include "Power_potion.h"

APower_potion::APower_potion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/DoveDove/potions/source/Potion_Potion04.Potion_Potion04'"));
	if (Mesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(Mesh.Object);
	}

	ItemName = FString(TEXT("Power_potion"));
}
