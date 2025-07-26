// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnActor.h"

// Sets default values
AItemSpawnActor::AItemSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawnActor::BeginPlay()
{
	Super::BeginPlay();
    SpawnRandomItem();
}

// Called every frame
void AItemSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemSpawnActor::SpawnRandomItem()
{
    if (ItemActorClasses.Num() == 0) return;

    // ¿¹: ·£´ý ¼±ÅÃ
    int32 Index = FMath::RandRange(0, ItemActorClasses.Num() - 1);
    TSubclassOf<AActor> SelectedClass = ItemActorClasses[Index];

    FVector SpawnLocation = GetActorLocation() ;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AActor>(SelectedClass, SpawnLocation, SpawnRotation, SpawnParams);
}

