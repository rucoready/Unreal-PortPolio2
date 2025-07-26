// Fill out your copyright notice in the Description page of Project Settings.


#include "DropActor.h"
#include "../MainCharacter/SwordCharacter.h"
#include "EngineUtils.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ADropActor::ADropActor()
{
	PrimaryActorTick.bCanEverTick = true;
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = sceneComp;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetupAttachment(RootComponent);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ADropActor::BeginPlay()
{
	Super::BeginPlay();
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADropActor::OnBeginOverlapDropActor);	
	myInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
}

// Called every frame
void ADropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (TActorIterator<ASwordCharacter>it(GetWorld()); it; ++it)
	{
		ASwordCharacter* swordCharacter = *it;
		if (swordCharacter)
		{
			float distance = FVector::Dist(this->GetActorLocation(), swordCharacter->GetActorLocation());
			if (distance <= absorptionArea)
			{
				FVector moveNewLoc = FMath::VInterpTo(GetActorLocation(), swordCharacter->GetActorLocation(), DeltaTime, 5.0f);
				SetActorLocation(moveNewLoc);
			}
		}
	}
}

void ADropActor::OnBeginOverlapDropActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("SwordCharacter"))
	{
		if (bMoney)
		{
			//magical big ass fucking number
			int32 randomMoney = FMath::RandRange(70, 100);
			myInstance->getMoney(randomMoney);
			UGameplayStatics::PlaySoundAtLocation(this, moneySound, GetActorLocation());
		}
		Destroy();
	}
}

