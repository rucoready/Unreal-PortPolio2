// Fill out your copyright notice in the Description page of Project Settings.


#include "moveMapActor.h"
#include "Components/BoxComponent.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../GameManager/StatGameInstance.h"


// Sets default values
AmoveMapActor::AmoveMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransformVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransformVolme"));
	RootComponent = TransformVolume;
	TransformVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));


}

// Called when the game starts or when spawned
void AmoveMapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AmoveMapActor::NotifyActorBeginOverlap(AActor* otherActor)
{
	ASwordCharacter* Chater = Cast<ASwordCharacter>(otherActor);

	if (Chater)
	{
		UStatGameInstance* GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (GameInstance)
		{
			GameInstance->GetPlayerStertSeting(Chater->playerCurrentHp, Chater->playerCurrentMp);
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("dungeonMap"));

	}
}

// Called every frame
void AmoveMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

