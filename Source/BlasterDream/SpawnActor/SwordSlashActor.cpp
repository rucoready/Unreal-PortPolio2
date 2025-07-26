// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordSlashActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "../MainCharacter/SwordCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"
#include "DownwardknockbackActor.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/GameplayStatics.h"



ASwordSlashActor::ASwordSlashActor()
{
	PrimaryActorTick.bCanEverTick = true;
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);

	collisionBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	collisionBoxComp->SetupAttachment(sceneComp);
	collisionBoxComp->SetRelativeLocation(FVector(158.0f, 0.0f, 113.0f));
	collisionBoxComp->SetRelativeScale3D(FVector(2.5474f, 0.89f, 3.508915f));
	collisionBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionBoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	collisionBoxComp->SetGenerateOverlapEvents(true);
	
	moveDirection = FVector::ZeroVector;
	targetLocation = FVector::ZeroVector;

	slashNA = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SlashNA"));
	slashNA->SetupAttachment(collisionBoxComp);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> slashNAs(TEXT("/Game/Luco/SwordSlash/NS_ASkillSlash.NS_ASkillSlash"));
	if (slashNAs.Succeeded())
	{
		slashNA->SetAsset(slashNAs.Object);
	}
}

void ASwordSlashActor::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		FHitResult hitResult;
		if (playerController->GetHitResultUnderCursor(ECC_Visibility, true, hitResult))
		{
			mouseWorldLocation = hitResult.ImpactPoint; 
			targetLocation = mouseWorldLocation;
		}
	}
	collisionBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordSlashActor::OnBeginOverlapCollision);
}

void ASwordSlashActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLoc = GetActorLocation();
	FVector direction = (targetLocation - currentLoc).GetSafeNormal();

	FVector deltaMove = direction * slashSpeed * DeltaTime;

	FHitResult hit;
	collisionBoxComp->MoveComponent(deltaMove, GetActorRotation(), true, &hit);

	// niagara rotation
	FRotator targetRotation = direction.Rotation();
	collisionBoxComp->SetWorldRotation(targetRotation);

}

void ASwordSlashActor::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		FString otherName = OtherActor->GetName();	
		if (otherName.Contains(TEXT("SwordCharacter")) || otherName.Contains(TEXT("SwordActor")) || otherName.Contains(TEXT("Crow")))
		{
			return;
		}
		FVector floorExplosionLoc = targetLocation - FVector(0, 0, 70);
		AActor* spawnInstance = GetWorld()->SpawnActor<AActor>(explosionActor, floorExplosionLoc, GetActorRotation());
		if (spawnInstance)
		{
			spawnInstance->SetActorScale3D(FVector(8.0f));
			spawnInstance->SetLifeSpan(0.2f);
			UParticleSystemComponent* particleInstance = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, targetLocation,
				FRotator::ZeroRotator, true);
			if (particleInstance)
			{
				particleInstance->SetRelativeScale3D(FVector(1.3f));
			}

			
		}

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			ASwordCharacter* swordCharacter = Cast<ASwordCharacter>(playerController->GetCharacter());
			if (swordCharacter)
			{
				UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
				swordCharacter->SlashHitCameraShake();
			}

			
		}

		Destroy();
	}
}


