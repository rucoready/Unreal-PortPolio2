// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"
#include "../MainCharacter/SwordCharacter.h"
#include "../GameManager/StatGameInstance.h"
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItems::AItems()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = sceneComp;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetupAttachment(RootComponent);
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnBeginOverlapDropActor);
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FHitResult HitResult;
	//FVector Start = GetActorLocation();
	//FVector End = Start + GetActorForwardVector();
	float SphereRadius = 200.0f;
		
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

	moveRotation(DeltaTime);
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

void AItems::PickItems()
{
	UStatGameInstance* GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (ItemName == FString(TEXT("RandomGear")))
	{
		int32 randNum = FMath::RandRange(0, 99);

		if (randNum <= 30)
		{
			ItemName = FString(TEXT("ringSteel"));
		}
		else if (randNum <= 50)
		{
			ItemName = FString(TEXT("ringBlue"));
		}
		else if (randNum <= 70)
		{
			ItemName = FString(TEXT("ringGold"));
		}
		else if (randNum <= 85)
		{
			ItemName = FString(TEXT("necklaceBlue"));
		}
		else if (randNum <= 99)
		{
			ItemName = FString(TEXT("necklaceGreen"));
		}
	}
	if (GameInstance)
	{
		//GameInstance->GetRay();
		GameInstance->AddItem(ItemName);
		Destroy();		
	}

}

void AItems::moveRotation(float time)
{
	FRotator NewRotation = FRotator(0, 90 * time, 0);
	AddActorLocalRotation(NewRotation);
}

void AItems::OnBeginOverlapDropActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("SwordCharacter"))
	{
		PickItems();
		UGameplayStatics::PlaySoundAtLocation(this, overlapSound, GetActorLocation());
		Destroy();
	}
}




