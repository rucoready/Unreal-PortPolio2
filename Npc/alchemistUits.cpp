// Fill out your copyright notice in the Description page of Project Settings.


#include "alchemistUits.h"

AalchemistUits::AalchemistUits()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ChildMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));
	if (ChildMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(ChildMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

}
