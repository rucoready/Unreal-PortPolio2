// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTestCrow.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "../MainCharacter/CharacterWidget/DamageWidget.h"
#include "Components/WidgetComponent.h"
#include "../SpawnActor/DamagePopupActor.h"
#include "EnemyUIDisplayWidget.h"
#include "EngineUtils.h"
#include "BasicSkeletonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "../MainCharacter/SwordCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../MainCharacter/HitInterface.h"

UEnemyUIDisplayWidget* AHitTestCrow::enemyUIDisplayWidgetInstance = nullptr;
AHitTestCrow* AHitTestCrow::currentWidgetOwner = nullptr;

// Sets default values
AHitTestCrow::AHitTestCrow()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	//SetRootComponent(capsulecomponent);

	scareCrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scareCrowMesh"));
	scareCrowMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> crowMesh(TEXT("/Game/Luco/scarecrow/source/scarecrow.scarecrow"));
	if (crowMesh.Succeeded())
	{
		scareCrowMesh->SetStaticMesh(crowMesh.Object);
	}
	scareCrowMesh->SetRelativeScale3D(FVector(2.0f));

	scareCrowMesh->SetRelativeLocation(FVector(0, 0, -90));
	scareCrowMesh->SetRelativeRotation(FRotator(0, -90, 0));
	scareCrowMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 0.7f));
	scareCrowMesh->SetCanEverAffectNavigation(false);

	originRot = scareCrowMesh->GetRelativeRotation();

	//Hud
	damageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	damageWidget->SetupAttachment(RootComponent);
	damageWidget->SetRelativeLocation(FVector(0, 0, 140));
	damageWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//damageAmount = 55.0f;

}

// Called when the game starts or when spawned
void AHitTestCrow::BeginPlay()
{
	Super::BeginPlay();
	
	//make mat instance
	if (scareCrowMesh)
	{
		originMat = scareCrowMesh->GetMaterial(0);
		if (originMat)
		{
			dynamicMat = scareCrowMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, originMat);
		}
	}
	if (damageWidget)
	{
		UUserWidget* damageW = damageWidget->GetUserWidgetObject();
		damageWidgetInstance = Cast<UDamageWidget>(damageW);
		
	}

	currentHp = maxHp;
	originSetLocation = GetActorLocation();
	originSetRotation = GetActorRotation();
}

// Called every frame
void AHitTestCrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (dynamicMat)
	{
		currentHitAmount = FMath::FInterpTo(currentHitAmount, targetHitAmount, DeltaTime, hitLerpSpeed);
		dynamicMat->SetScalarParameterValue(FName("HitAmount"), currentHitAmount);
	}

	float distanceFromOriginSetting = FVector::Dist(GetActorLocation(), originSetLocation);
	if (distanceFromOriginSetting > 700.f)
	{
		
		GetCharacterMovement()->StopMovementImmediately();
		SetActorLocation(originSetLocation);
		SetActorRotation(originSetRotation);

		
	}
	
	
}

// Called to bind functionality to input
void AHitTestCrow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHitTestCrow::GetHit(const FVector& ImpactPoint)
{
	if (GEngine && GEngine->GameViewport)
	{
		UWorld* world = GEngine->GameViewport->GetWorld();
		if (world)
		{
			DrawDebugSphere(
				world,
				ImpactPoint,
				10.f,          // 반지름
				12,            
				FColor::Purple,   // 색깔
				false,         // 영구 표시 여부
				2.0f           // 지속 시간
			);
		}
	}
	DummyBeAttacked();
}

void AHitTestCrow::DummyBeAttacked()
{
	//change Radom Pitch Code
	FRotator currentRot = scareCrowMesh->GetRelativeRotation();
	float randomRoll = FMath::RandRange(5.f, 25.f);

	FRotator plusRollRot = originRot;
	plusRollRot.Roll -= randomRoll;

	//SetActorRotation(plusPitchRot);
	scareCrowMesh->SetRelativeRotation(plusRollRot);

	GetWorld()->GetTimerManager().SetTimer(th_HandlePitch10Plus, [this]()
		{
			scareCrowMesh->SetRelativeRotation(originRot);
			//SetActorRotation(currentRot);
		}, 0.2f, false);

	//change DynamicMaterial Code
	targetHitAmount = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(th_HandleChangeOriginMat, [this]()
		{
			targetHitAmount = 0.0f;
		}, 0.2f, false);
	//howHud(damageAmount);

	//show enemydisplayUIWidget
	if (enemyUIDisplayWidgetInstance && ::IsValid(enemyUIDisplayWidgetInstance))
	{
		if (enemyUIDisplayWidgetInstance->IsInViewport())
		{
			enemyUIDisplayWidgetInstance->RemoveFromParent();
		}
	}

	enemyUIDisplayWidgetInstance = nullptr;

	enemyUIDisplayWidgetInstance = CreateWidget<UEnemyUIDisplayWidget>(GetWorld(), enemyDisplayWidget);

	if (IsValid(enemyUIDisplayWidgetInstance))
	{
		enemyUIDisplayWidgetInstance->AddToViewport();

		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				if (IsValid(enemyUIDisplayWidgetInstance))
				{
					enemyUIDisplayWidgetInstance->updateHpBar(monsterName, currentHp, maxHp);
				}
			});
	}


	currentWidgetOwner = this;
	GetWorld()->GetTimerManager().ClearTimer(th_HideUIAfterDelay);
	GetWorld()->GetTimerManager().SetTimer(th_HideUIAfterDelay, [this]()
		{
			if (this == currentWidgetOwner && enemyUIDisplayWidgetInstance && enemyUIDisplayWidgetInstance->IsInViewport())
			{
				enemyUIDisplayWidgetInstance->RemoveFromParent();
				enemyUIDisplayWidgetInstance = nullptr;
				currentWidgetOwner = nullptr;
			}
		}, 3.0f, false);
	
	for (TActorIterator<ABasicSkeletonEnemy> it(GetWorld()); it; ++it)
	{
		ABasicSkeletonEnemy* skeletons = *it;
		if (skeletons)
		{
			if (skeletons->enemyUIDisplayWidgetInstance && ::IsValid(skeletons->enemyUIDisplayWidgetInstance))
			{
				if (skeletons->enemyUIDisplayWidgetInstance->IsInViewport())
				{
					skeletons->enemyUIDisplayWidgetInstance->RemoveFromParent();
					skeletons->enemyUIDisplayWidgetInstance = nullptr;
					skeletons->currentWidgetOwner = nullptr;
				}
			}
		}
	}
}

void AHitTestCrow::ShowHud(float damage)
{
	if (!damagePopupActor)
	{
		return;
	}

	FVector spawnLoc = GetActorLocation() + FVector(0, 0, 140);
	FRotator spawnRot = FRotator::ZeroRotator;

	ACharacter* playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	swordCharacter = Cast<ASwordCharacter>(playerChar);

	ADamagePopupActor* popupActor = GetWorld()->SpawnActor<ADamagePopupActor>(damagePopupActor, spawnLoc, spawnRot);
	if (popupActor)
	{
		popupActor->InitDamage(damage, swordCharacter->bCritical);
		
	}
	currentHp -= damage;
	
}

void AHitTestCrow::TakeDamage()
{
	
}


