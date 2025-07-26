// Fill out your copyright notice in the Description page of Project Settings.


#include "Propital_potion.h"

APropital_potion::APropital_potion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/DoveDove/potions/source/Potion_Potion06.Potion_Potion06'"));
	if (Mesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(Mesh.Object);
	}

	ItemName = FString(TEXT("Propital_potion"));
}
