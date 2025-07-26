// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrangonAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../MainCharacter/CharacterWidget/DragonWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "SwordPlayController.h"
#include "DragonController.h"
#include "Camera/CameraComponent.h"

// Sets default values
ADragonCharacter::ADragonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>dragonAsset(TEXT("/Game/FourEvilDragonsPBR/Meshes/DragonTheUsurper/DragonTheUsurperSK"));
	if (dragonAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(dragonAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetWorldScale3D(FVector(0.5f));
	}
	//isomatric view setting
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SetRootComponent(GetCapsuleComponent());
	//camera
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->SetUsingAbsoluteRotation(true);
	springArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	springArmComponent->TargetArmLength = 1300.f;
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArmComponent);

	//minimap
	minimapCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArmComp"));
	minimapCameraBoom->SetupAttachment(RootComponent);
	minimapCameraBoom->SetWorldRotation(FRotator(-90.0f, 45.0f, 0.0f));

	minimapCameraBoom->TargetArmLength = 900.0f;
	minimapCameraBoom->bUsePawnControlRotation = false;
	minimapCameraBoom->bInheritPitch = false;
	minimapCameraBoom->bInheritRoll = false;
	minimapCameraBoom->bInheritYaw = false;

	minimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureMinimap"));
	minimapCapture->SetupAttachment(minimapCameraBoom);
	minimapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	minimapCapture->OrthoWidth = 1700.0f;



	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> renderObj(TEXT("/Game/Luco/MiniMap/CRT_Minimap.CRT_Minimap"));
	if (renderObj.Succeeded())
	{
		minimapCapture->TextureTarget = renderObj.Object;
	}
	minimapSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	minimapSprite->SetupAttachment(GetMesh());

	//polymorf hold camera
	polyMolfHoldCameraActor = CreateDefaultSubobject<AActor>(TEXT("HoldCameraActor"));
	polyMolfHoldCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HoldCamera"));
	polyMolfHoldCamera->SetupAttachment(RootComponent);
	polyMolfHoldCamera->SetRelativeLocation(FVector(0.f, 0.f, 650.f));
	polyMolfHoldCamera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

}

void ADragonCharacter::BeginPlay()
{
	Super::BeginPlay();
	dragonAnimInstance = Cast<UDrangonAnimInstance>(GetMesh()->GetAnimInstance());
	animInstance = GetMesh()->GetAnimInstance();
	if (cameraComponent)
	{
		currentFOV = cameraComponent->FieldOfView;
		targetFOV = currentFOV;
	}
	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
	ShowViewPortMainWidget();
	dragonController = Cast<ADragonController>(GetController());
	
}

void ADragonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (cameraComponent)
	{
		currentFOV = FMath::FInterpTo(currentFOV, targetFOV, DeltaTime, fovInterpSpeed);
		cameraComponent->SetFieldOfView(currentFOV);
	}
	//z scale up
	if (bInterpZ)
	{
		zInterpElapsed += DeltaTime;
		float progressAlpha = FMath::Clamp(zInterpElapsed / zInterpTime, 0.f, 1.f);
		FVector NewLocation = FMath::Lerp(startLoc, flyingTargetLoc, progressAlpha);
		SetActorLocation(NewLocation);

		if (progressAlpha >= 1.f)
		{
			bInterpZ = false;
		}
	}
	//x y up
	if (bFlyingMoveToTarget)
	{
		FVector currentLoc = GetActorLocation();
		FVector direction = (flyingTargetLocation - currentLoc).GetSafeNormal();
		float flyingSpeed = 600.f;

		AddMovementInput(direction, 1.f);


		if (FVector::DistSquared(currentLoc, flyingTargetLocation) < FMath::Square(100.f))
		{
			bFlyingMoveToTarget = false;
		}
	}
	else if(!bFlyingMoveToTarget)
	{
		bFlyingMoveToTarget = false;
	}
	
	
}

void ADragonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADragonCharacter::ToggleFlying()
{
	if (!bToggleFly)
	{
		return;
	}
	bToggleFly = false;


	if (bFlying)
	{
		PlayAnimMontage(takeOnMontage);
	}
	else
	{
		PlayAnimMontage(takeOffMontage);
	}

	GetWorld()->GetTimerManager().SetTimer(th_ToggleFly, FTimerDelegate::CreateLambda([this]()
		{
			bToggleFly = true;
		}),
		flyToggleCoolDown, false);
}


void ADragonCharacter::OnTakeOff()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.f;
	bFlying = true;
	dragonAnimInstance->bGliding = true;
	targetFOV = 110.f;  
}

void ADragonCharacter::OnTakeOn()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->GravityScale = 1.f;
	bFlying = false;
	dragonAnimInstance->bGliding = false;
	targetFOV = 90.f;
}

void ADragonCharacter::DragonFlying()
{
	startLoc = GetActorLocation();
	flyingTargetLoc = startLoc + FVector(0.f, 0.f, 300.f);
	zInterpElapsed = 0.f;
	bInterpZ = true;
	targetFOV = 110.f;
	fovInterpSpeed = 1.5f;
	OnTakeOff();
}

void ADragonCharacter::DragonStopFlying()
{
	FVector LaunchVelocity = FVector(0.f, 0.f, -600.f); 
	LaunchCharacter(LaunchVelocity, true, true);

}

void ADragonCharacter::DragonQAttack()
{
	PlayAnimMontage(qAnim);
}

void ADragonCharacter::DragonWAttack()
{
	if (!dragonOrbClass) return;
	APlayerController* pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc) return;
	if (!pc->GetHitResultUnderCursor(ECC_Visibility, false, orbHitResult)) return;

	UGameplayStatics::SpawnSound2D(this, dragonOrbChargeSound);

	FVector orbFireLoc = GetMesh()->GetSocketLocation(TEXT("FireSocket"));
	FVector orbTargetLoc = orbHitResult.ImpactPoint - orbFireLoc;
	FRotator orbFireRot = orbTargetLoc.Rotation(); 

	FActorSpawnParameters orbSpawnParams;
	orbSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	orbSpawnParams.Owner = this;
	dragonOrb = GetWorld()->SpawnActor<ADragonWOrb>(dragonOrbClass, orbFireLoc, orbFireRot, orbSpawnParams);
	dragonOrb->SetLifeSpan(5.0);
}

void ADragonCharacter::MoveToInAir(const FVector& target)
{
	flyingTargetLocation = target;
	bFlyingMoveToTarget = true;
}

void ADragonCharacter::ShowViewPortMainWidget()
{
	APlayerController* baseController = UGameplayStatics::GetPlayerController(this, 0);
	if (baseController)
	{
		dragonWidget = CreateWidget<UDragonWidget>(baseController, dragonWidgetClass);
		dragonWidget->AddToViewport();
		
	}
}

void ADragonCharacter::UpdateLookAngleToMouse()
{
	APlayerController* pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!pc) return;

	FHitResult HitResult;
	if (!pc->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		return;

	FVector forward = GetActorForwardVector().GetSafeNormal2D();
	FVector toMouse = (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal2D();

	float angle = FMath::Acos(FVector::DotProduct(forward, toMouse));
	angle = FMath::RadiansToDegrees(angle);

	float CrossZ = FVector::CrossProduct(forward, toMouse).Z;
	float signedAngle = angle * (CrossZ > 0.f ? 1.f : -1.f);

	signedAngle = FMath::Clamp(signedAngle, -180.f, 180.f);
	lookAngle = signedAngle;
	dragonAnimInstance->bWAiming = true;
}

void ADragonCharacter::UpdateCancleLookAngleToMouse()
{
	dragonAnimInstance->bWAiming = false;
}

float ADragonCharacter::GetLookAngle() const
{
	return lookAngle;
}

void ADragonCharacter::WOrbHitCameraShake()
{
	if (orbHitCameraShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(orbHitCameraShake);
		}
	}
}

void ADragonCharacter::DragonVisible()
{
	GetMesh()->SetVisibility(true);
}

void ADragonCharacter::DragonUnVisible()
{
	GetMesh()->SetVisibility(false);
}

void ADragonCharacter::PolyMolfCancle()
{
	if (!IsValid(this))
	{
		return;
	}

	if (IsValid(polymolfCancleMontage))
	{
		PlayAnimMontage(polymolfCancleMontage);
	}
}

void ADragonCharacter::PolyMolfCancleAnimationFinished()
{
	//new poccess
	FVector dragonSpawnLocation = GetActorLocation();
	FRotator dragonspawnRotation = GetActorRotation();
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		playerController->UnPossess();
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		this->SetActorTickEnabled(false);
		dragonWidget->SetVisibility(ESlateVisibility::Hidden);

		// player On
		rememberedSwordCharacter->SetActorLocation(GetActorLocation());
		rememberedSwordCharacter->SetActorRotation(GetActorRotation());
		rememberedSwordCharacter->SetActorHiddenInGame(false);
		rememberedSwordCharacter->SetActorEnableCollision(true);
		rememberedSwordCharacter->SetActorTickEnabled(true);

		// player Controller On
		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		if (localPlayer)
		{
			localPlayer->PlayerController = rememberedSwordCharacter->rememberedSwordController;
		}

		rememberedSwordCharacter->rememberedSwordController->Player = localPlayer;
		rememberedSwordCharacter->rememberedSwordController->Possess(rememberedSwordCharacter);

		// 원래 사람 복구
		rememberedSwordCharacter->PolyMolfRetrunPlayer();
		rememberedSwordCharacter->GetMesh()->SetVisibility(true);
		rememberedSwordCharacter->overlapSword->SetVisibility(true);
		rememberedSwordCharacter->ShowAllWidget();
		rememberedSwordCharacter->skillCamera->Deactivate();
		rememberedSwordCharacter->CameraComponent->Activate();
		rememberedSwordCharacter->rememberedSwordController->SetViewTargetWithBlend(rememberedSwordCharacter, 0.5f);
		rememberedSwordCharacter->playerCurrentForce = 0.0f;
		rememberedSwordCharacter->GetWorldTimerManager().ClearTimer(rememberedSwordCharacter->th_decreaseForce);
		
	}

	
	
}

void ADragonCharacter::PossessSetting()
{
	
}

void ADragonCharacter::RemoveDragonWidget()
{
	if (dragonWidget && dragonWidget->IsInViewport())
	{
		dragonWidget->RemoveFromParent();
		dragonWidget = nullptr;

	}
}













