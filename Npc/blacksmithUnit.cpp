// Fill out your copyright notice in the Description page of Project Settings.


#include "blacksmithUnit.h"

AblacksmithUnit::AblacksmithUnit()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChildMesh(TEXT("/Game/ParagonGideon/Characters/Heroes/Gideon/Skins/Inquisitor/Meshes/Gideon_Inquisitor.Gideon_Inquisitor"));
	if (ChildMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ChildMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

}
