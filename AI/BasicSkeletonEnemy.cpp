


#include "BasicSkeletonEnemy.h"
#include "Components/CapsuleComponent.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SkeletonAIController.h"
#include "../MainCharacter/CharacterWidget/DamageWidget.h"
#include "Components/WidgetComponent.h"
#include "../SpawnActor/DamagePopupActor.h"
#include "EnemyUIDisplayWidget.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "../Task/AIStateEnum.h"
#include "SkeletonHPBarWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SkeletonAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../MainCharacter/HitInterface.h"

UEnemyUIDisplayWidget* ABasicSkeletonEnemy::enemyUIDisplayWidgetInstance = nullptr;
ABasicSkeletonEnemy* ABasicSkeletonEnemy::currentWidgetOwner = nullptr;

ABasicSkeletonEnemy::ABasicSkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	//autopoccess
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASkeletonAIController::StaticClass();

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetWorldScale3D(FVector(1.1f));

	sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	sword->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	
	swordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	swordCollision->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	swordCollision->SetRelativeLocation(FVector(0.0f, -0.1f, -4.4f));
	swordCollision->SetWorldScale3D(FVector(0.5f, 1.0f, 2.8f));
	motionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	//Hud
	damageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidget"));
	damageWidget->SetupAttachment(RootComponent);
	damageWidget->SetRelativeLocation(FVector(0, 0, 140));
	damageWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//floatingWidget
	hpFloatingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("floatingWidget"));
	hpFloatingWidget->SetupAttachment(RootComponent);
	hpFloatingWidget->SetRelativeLocation(FVector(0, 0, 125));
	hpFloatingWidget->SetWorldScale3D(FVector(1.0, 0.23, 0.03));
	hpFloatingWidget->SetWidgetSpace(EWidgetSpace::Screen);
	hpFloatingWidget->SetVisibility(false);

	//RVO Active
	GetCharacterMovement()->bUseRVOAvoidance = true;
	//Avoid Radius
	GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f; 
	GetCharacterMovement()->AvoidanceWeight = 0.5f;

}

void ABasicSkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
	swordCollision->OnComponentBeginOverlap.AddDynamic(this, &ABasicSkeletonEnemy::OnBeginOverlapSwordCollision);
	//swordCharacter = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	aiController = Cast<ASkeletonAIController>(GetController());
	currentHp = maxHp;
	skeletonHpBarWidget = Cast<USkeletonHPBarWidget>(hpFloatingWidget->GetUserWidgetObject());
	CollisionOff();
}

void ABasicSkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	skeletonHpBarWidget->updateHpBar(currentHp, maxHp);
}

void ABasicSkeletonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicSkeletonEnemy::OnBeginOverlapSwordCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString hitActorName = OtherActor->GetName();
	ASwordCharacter* castedSwordCharacter = Cast<ASwordCharacter>(OtherActor);
	if (castedSwordCharacter)
	{
		lastHitSwordCharacter = castedSwordCharacter;
		castedSwordCharacter->GetDamage(attackDamage);
	}
	
	

}

void ABasicSkeletonEnemy::PlayAttackAnimation()
{

	int32 randomAttack = FMath::RandRange(1, 3);
	switch (randomAttack)
	{
		case 1:
			MotionWarpingPlayer();
			PlayAnimMontage(attack01);
			break;
		case 2:
			MotionWarpingPlayer();
			PlayAnimMontage(attack02);
			break;
		case 3:
			MotionWarpingPlayer();
			PlayAnimMontage(attack03);
			break;
		default:
			break;
	}
}

void ABasicSkeletonEnemy::MotionWarpingPlayer()
{
	FVector playerLocation = aiController->GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
	if (motionWarpComponent)
	{
		FVector enemyLocation = GetActorLocation(); 
		FVector playerDirection = playerLocation - enemyLocation;

		//only yaw
		float yaw = playerDirection.Rotation().Yaw;
		FRotator playerRotation(0.f, yaw, 0.f);

		FTransform playerTransform(playerRotation, playerLocation);
		motionWarpComponent->AddOrUpdateWarpTargetFromTransform(TEXT("PlayerTarget"), playerTransform);
	}
}

void ABasicSkeletonEnemy::CollisionOff()
{
	swordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bHit = false;
}

void ABasicSkeletonEnemy::CollisionOn()
{
	swordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABasicSkeletonEnemy::ShowHud(float damage)
{
	if (!damagePopupActor)
    {
        return;
    }
	ACharacter* playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	swordCharacter = Cast<ASwordCharacter>(playerChar);
	FVector spawnLoc = GetActorLocation() + FVector(0, 0, 140);
	FRotator spawnRot = FRotator::ZeroRotator;
	ADamagePopupActor* popupActor = GetWorld()->SpawnActor<ADamagePopupActor>(damagePopupActor, spawnLoc, spawnRot);
	if (popupActor)
	{
		if (swordCharacter)
		{
			popupActor->InitDamage(damage, swordCharacter->bCritical);
		}
		else
		{
			popupActor->InitDamage(damage, false);
		}
		
	}
	currentHp = FMath::Clamp(currentHp - damage, 0.0f, maxHp);
	pendingDamageCount++;

	if (!bDamaged)
	{
		bDamaged = true;
	}
}

void ABasicSkeletonEnemy::GetHit(const FVector& ImpactPoint)
{
	
	SkeletonBeAttacked();
	
}

void ABasicSkeletonEnemy::SkeletonBeAttacked()
{
	
	if (enemyUIDisplayWidgetInstance && ::IsValid(enemyUIDisplayWidgetInstance))
	{
		if (enemyUIDisplayWidgetInstance->IsInViewport())
		{
			enemyUIDisplayWidgetInstance->RemoveFromParent();
		}
	}
	
	currentWidgetOwner = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(th_HideUIAfterDelay);

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
	GetWorld()->GetTimerManager().SetTimer(th_HideUIAfterDelay, [this]()
		{
			if (this == currentWidgetOwner && enemyUIDisplayWidgetInstance && enemyUIDisplayWidgetInstance->IsInViewport())
			{
				enemyUIDisplayWidgetInstance->RemoveFromParent();
				enemyUIDisplayWidgetInstance = nullptr;
				currentWidgetOwner = nullptr;
			}
		}, 3.0f, false);

	for (TActorIterator<AHitTestCrow> it(GetWorld()); it; ++it)
	{
		AHitTestCrow* crows = *it;
		if (crows)
		{
			if (crows->enemyUIDisplayWidgetInstance && ::IsValid(crows->enemyUIDisplayWidgetInstance))
			{
				if (crows->enemyUIDisplayWidgetInstance->IsInViewport())
				{
					crows->enemyUIDisplayWidgetInstance->RemoveFromParent();
					crows->enemyUIDisplayWidgetInstance = nullptr;
					crows->currentWidgetOwner = nullptr;
				}
			}
		}
	}
}

void ABasicSkeletonEnemy::DamagedMontagePlay()
{
	if (damagedMontage)
	{
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance)
		{
	
			animInstance->Montage_Play(damagedMontage);

		
			FOnMontageEnded endDelegate;
			endDelegate.BindUObject(this, &ABasicSkeletonEnemy::OnDamagedMontageEnded);
			animInstance->Montage_SetEndDelegate(endDelegate, damagedMontage);
		}
	}
}

void ABasicSkeletonEnemy::SpawnArrow()
{
	FVector spawnLoc = GetMesh()->GetSocketLocation(TEXT("WeaponSocket"));
	if (motionWarpComponent)
	{
		const FMotionWarpingTarget* target = motionWarpComponent->FindWarpTarget(TEXT("PlayerTarget"));
		if (target)
		{
			FVector targetLoc = target->GetLocation();
			FVector direction = targetLoc - spawnLoc;
			FRotator spawnRot = direction.Rotation();

			FActorSpawnParameters spawnParams;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ABowSkeletonProjectTile* SpawnedArrow = GetWorld()->SpawnActor<ABowSkeletonProjectTile>(arrowProjectTile,spawnLoc,spawnRot,spawnParams);
			if (SpawnedArrow)
			{
				SpawnedArrow->SetLifeSpan(5.0f);  
			}
		}
	}
}

void ABasicSkeletonEnemy::OnDamagedMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bDamaged = false;
}








