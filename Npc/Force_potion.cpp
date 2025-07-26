// Fill out your copyright notice in the Description page of Project Settings.


#include "Force_potion.h"

AForce_potion::AForce_potion()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/DoveDove/potions/source/Potion_Potion03.Potion_Potion03'"));
	if (Mesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(Mesh.Object);
	}

	ItemName = FString(TEXT("Force_potion"));
}
