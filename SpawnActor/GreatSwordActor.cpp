// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSwordActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGreatSwordActor::AGreatSwordActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);

	swordComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	swordComp->SetupAttachment(RootComponent);
	swordComp->SetMobility(EComponentMobility::Movable);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> swordMesh(TEXT("/Game/Luco/GreatSword/SM_GreatSword.SM_GreatSword"));
	if (swordMesh.Succeeded())
	{
		swordComp->SetStaticMesh(swordMesh.Object);
	}
	swordComp->SetRelativeScale3D(FVector(3.0f));
	swordComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AGreatSwordActor::BeginPlay()
{
	Super::BeginPlay();
	SwingSword();
}

// Called every frame
void AGreatSwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotatingNow)
	{
		elapsedTime += DeltaTime;

		float alphaTime = FMath::Clamp(elapsedTime / rotationDuration, 0.0f, 1.0f);

		//move
		float moveAlphaTime = FMath::InterpEaseIn(0.0f, 1.0f, alphaTime, slowExponent);

		float targetRot = totalRot * moveAlphaTime;

		float rotStep = targetRot - currentRot;

		swordComp->AddLocalRotation(FRotator(rotStep, 0.f, 0.f));

		currentRot = targetRot;

		if (alphaTime >= 1.0f)
		{
			bIsRotatingNow = false;
		}
	}	
}

void AGreatSwordActor::SwingSword()
{
	bIsRotatingNow = true;
	elapsedTime = 0.0f;      
	currentRot = 0.0f;       
	totalRot = -270.0f;       
	rotationDuration = 0.9f; 
	slowExponent = 11.0f;
}
