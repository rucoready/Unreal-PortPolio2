// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulActor.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "../MainCharacter/SwordCharacter.h"
#include "../MainCharacter/DragonCharacter.h"
#include "GameFramework/PlayerController.h"

ASoulActor::ASoulActor()
{
	PrimaryActorTick.bCanEverTick = true;
	soulWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SoulWidget"));
	soulWidget->SetupAttachment(RootComponent);
	soulWidget->SetWidgetSpace(EWidgetSpace::Screen);
	soulWidget->SetDrawSize(FVector2D(64.f, 64.f));
    SetRootComponent(soulWidget);
}

void ASoulActor::BeginPlay()
{
	Super::BeginPlay();
	playerCon = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ASwordCharacter* swordChar = Cast<ASwordCharacter>(playerCon->GetPawn());
}

void ASoulActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!playerCon) return;

    ASwordCharacter* swordChar = Cast<ASwordCharacter>(playerCon->GetPawn());
    APawn* controlledPawn = playerCon->GetPawn();
    if (controlledPawn && controlledPawn->IsA(ADragonCharacter::StaticClass()))
    {
        Destroy();
        return;
    }
    if (!swordChar || !swordChar->characterWidget) return;

    FVector2D holeScreenPos = swordChar->characterWidget->GetSoulHoleScreentPosition();

    FVector worldOrigin, worldDirection;
    if (playerCon->DeprojectScreenPositionToWorld(holeScreenPos.X, holeScreenPos.Y, worldOrigin, worldDirection))
    {
        float distance = 1000.f;
        FVector targetWorldLocation = worldOrigin + worldDirection * distance;

        FVector currentLocation = GetActorLocation();
        FVector direction = (targetWorldLocation - currentLocation).GetSafeNormal();

        elasedTime += DeltaTime;
        float startSpeed = 400.f;
        float endSpeed = 2100.f;
        float alpha = FMath::Clamp(elasedTime / movingDuration, 0.f, 1.f);
        float exponent = 5.f;

        float speed = FMath::InterpEaseIn(startSpeed, endSpeed, alpha, exponent);

        float distanceToTarget = FVector::Dist(currentLocation, targetWorldLocation);
        float moveStep = speed * DeltaTime;

        if (moveStep >= distanceToTarget)
        {
            SetActorLocation(targetWorldLocation);
            swordChar->characterWidget->SoulCollection();
            swordChar->characterWidget->IncreaseSoulCollectionBar();
            UGameplayStatics::SpawnSound2D(this, getSoulSound);
            Destroy();
            return;
        }
        else
        {
            FVector newLocation = currentLocation + direction * moveStep;
            SetActorLocation(newLocation);
        }
    }
}



