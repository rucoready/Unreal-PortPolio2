// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Animation/AnimMontage.h"
#include "Components/WidgetComponent.h"
#include "SwordPlayController.h"
#include "Animation/AnimSequence.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"
#include "Engine/DecalActor.h"
#include "../SpawnActor/IndicatorDecalActor.h"
#include "../SpawnActor/AoeCircleDecal.h"
#include "../SpawnActor/SwordActor.h"
#include "Components/DecalComponent.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MainWidget.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "PaperSpriteComponent.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../GameManager/ProjectSaveGame.h"
#include "LandscapeComponent.h"
#include "Landscape.h"
#include "LandscapeLayerInfoObject.h"
#include "../MainCharacter/CharacterWidget/PauseWidget.h"
#include "LandscapeInfo.h"
#include "CineCameraComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "DragonCharacter.h"
#include "DragonController.h"
#include "Camera/CameraActor.h"
#include "../MainCharacter/CharacterWidget/DragonWidget.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASwordCharacter::ASwordCharacter()
{
	bHasAttackMode = false;



	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = originArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	// Camera Hit
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->ProbeSize = 12.0f;
	SpringArmComponent->ProbeChannel = ECC_Camera;

	// Camera Move
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 10.0f;

	

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>swordPlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ElfArden/BaseMesh/SK_ElfArden.SK_ElfArden'"));
	if (swordPlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(swordPlayerMesh.Object);
	}

	//set mesh location and rotation
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	motionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	ghostTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GhostTrailComponent"));
	ghostTrailComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> GhostTrailSystemAsset(TEXT("/Game/Luco/GhostTrail/NA_GhostTrail.NA_GhostTrail"));

	if (GhostTrailSystemAsset.Succeeded())
	{
		ghostTrailComponent->SetAsset(GhostTrailSystemAsset.Object);
		ghostTrailComponent->SetAutoActivate(false);
		HideGhostTrail();
	}



	//overlapSword
	overlapSword = CreateDefaultSubobject<UChildActorComponent>(TEXT("overlapSwordMesh"));
	overlapSword-> SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	overlapSword->SetWorldScale3D(FVector(1.3f, 1.3f, 1.3f));
	overlapSword->SetVisibility(false);
	

	/////////////////////////////////////////////////////////////////////////////
	forceAwakeParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AWakeForceParticle"));
	forceAwakeParticle->SetupAttachment(CameraComponent);
	forceAwakeParticle->SetRelativeLocation(FVector(93.0, 0, 0));
	forceAwakeParticle->SetWorldScale3D(FVector(1.3f, 1.2f, 1.3f));

	forceAwakeParticle->bAutoActivate = false; 
	
	forceAwakeLightning = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AWakeForceLightning"));
	forceAwakeLightning->SetupAttachment(GetMesh());
	forceAwakeLightning->SetRelativeLocation(FVector(0,0,90));
	forceAwakeLightning->SetWorldScale3D(FVector(0.5f));
	forceAwakeLightning->SetVisibility(false);
	backToOriginMaxWalkSpeed();

	forceBodyLightning = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ForceBodyLightning"));
	forceBodyLightning->SetupAttachment(GetMesh());
	forceBodyLightning->SetVisibility(false);


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
	minimapCapture->ShowOnlyComponents.Add(minimapSprite);

	
	
	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> renderObj(TEXT("/Game/Luco/MiniMap/CRT_Minimap.CRT_Minimap"));
	if (renderObj.Succeeded())
	{
		minimapCapture->TextureTarget = renderObj.Object;
	}
	minimapSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	minimapSprite->SetupAttachment(GetMesh());


	//cinecamera
	skillCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SkillCamera"));
	skillCamera->SetupAttachment(RootComponent);
	skillCamera->SetRelativeLocation(FVector(-70.f, 230.f, 110.f));
	skillCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));


}

// Called when the game starts or when spawned
void ASwordCharacter::BeginPlay()
{
	Super::BeginPlay();
	getDamageDatableValue(14.0f);

}


// Called every frame
void ASwordCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float MpRegenRate = 230.0f;  // 초당 회복량
	playerCurrentMp = FMath::Clamp(playerCurrentMp + MpRegenRate * DeltaTime, 0.0f, playerMaxMp);
	//특수 아이템 사용시 

	if (usingP_potion)
	{
		usingPropital(DeltaTime);
	}
	//------
	if (bForceMode != bPrevForceMode)
	{
		if (bForceMode)
		{
			if (characterWidget)
			{
				characterWidget->OnFinishCtrlAnim();
				characterWidget->StartForce();
			}
		}
		bPrevForceMode = bForceMode;
	}

	if (!bForceMode)
	{
		float forceRegenRate = 10.0f;
		playerCurrentForce = FMath::Clamp(playerCurrentForce + forceRegenRate * DeltaTime, 0.0f, playerMaxForce);
		if (playerCurrentForce >= playerMaxForce)
		{
			if (!bPlayedMaxForceSound)
			{
				if (characterWidget)
				{
					characterWidget->SetForceWidgetNiagara();
				}
				UGameplayStatics::PlaySound2D(this, maxForceChargingSound);
				bPlayedMaxForceSound = true;
			}
		}
		else
		{
			if (characterWidget)
			{
				if (characterWidget)
				{
					characterWidget->HideForceWidget();
				}
			}
			bPlayedMaxForceSound = false;
		}
	}
	
	
	
	updateUI();
	if (!bCanUseSkillQ)
	{
		currentSkillQTime += DeltaTime;
		if (currentSkillQTime >= skillQCoolTime)
		{
			bCanUseSkillQ = true;
			currentSkillQTime = 0.0f;
		}
	}

	if (!bCanUseSkillW)
	{
		currentSkillWTime += DeltaTime;
		if (currentSkillWTime >= skillWCoolTime)
		{
			bCanUseSkillW = true;
			currentSkillWTime = 0.0f;
		}
	}

	if (!bCanUseSkillE)
	{
		currentSkillETime += DeltaTime;
		if (currentSkillETime >= skillECoolTime)
		{
			bCanUseSkillE = true;
			currentSkillETime = 0.0f;
		}
	}

	if (!bCanUseSkillR)
	{
		currentSkillRTime += DeltaTime;
		if (currentSkillRTime >= skillRCoolTime)
		{
			bCanUseSkillR = true;
			currentSkillRTime = 0.0f;
		}
	}


	if (!bCanUseSkillA)
	{
		currentSkillATime += DeltaTime;
		if (currentSkillATime >= skillACoolTime)
		{
			bCanUseSkillA = true;
			currentSkillATime = 0.0f;
		}
	}
	
	if (swordPlayerController)
	{
		swordPlayerController->getMousePosition();
		swordPlayerController->getCurrentActorLocAndRot();
	}
	

	//decal update
	if (swordPlayerController)
	{

		if (swordPlayerController->bClickRHolding)
		{
			FHitResult spawnLoc = swordPlayerController->skillMouseCursorHit;
			spawnImpactDecal->SetActorLocation(spawnLoc.ImpactPoint);

		}
		
		
	}
	



	if (bAOESpawnTime && bHovering)
	{
		FHitResult spawnLoc = swordPlayerController->skillMouseCursorHit;
		spawnDecal->SetActorLocation(spawnLoc.ImpactPoint);
	}

	if (spawnAoeR)
	{
		
		FVector spawnLoc = GetActorLocation();
		spawnAoeR->SetActorLocation(spawnLoc);
	}

	checkInAOEDecalRange();
	StopAOverHeight();
	HoveringCheck();
	if (bSSkillMoving)
	{
		SkillSMoving(DeltaTime);
	}


	//iosomatric view camera arm setting
	UpdateCameraArmLength(DeltaTime);
	//CheckWallHitFlag();


	if (InGameModeUserWidgetSet)
	{
		SetUpdateGameModeGamePlayWidget();
	}
	
	//overlay
	if (bloodWidget && playerMaxHp > 0.f)
	{
		float hpPercent = playerCurrentHp / playerMaxHp;
		if (playerCurrentHp <= 0.f)
		{
			// hp 0
			if (bloodWidget->GetVisibility() != ESlateVisibility::Hidden)
			{
				bloodWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			bWasLowHp = false; 
		}
		else if (hpPercent <= 0.35f)
		{
			if (!bWasLowHp)
			{
				bWasLowHp = true;
				if (bloodWidget->GetVisibility() != ESlateVisibility::Visible)
				{
					bloodWidget->SetVisibility(ESlateVisibility::Visible);
				}

				bloodWidget->PlayBloodAnimation();  // toggle On
				UGameplayStatics::SpawnSound2D(this, lowHPVoice);
			}
		}
		else
		{
			if (bloodWidget->GetVisibility() != ESlateVisibility::Hidden)
			{
				bloodWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			if (bWasLowHp)
			{
				bWasLowHp = false;
				bloodWidget->PlayBloodAnimation();  // toggle Off
			}
		}
	}
	
}

// Called to bind functionality to input
void ASwordCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASwordCharacter::meleeAttack()
{
	

	if (!bForceMode)
	{
		if (bHasDelayAttack == false)
		{
			
			switch (comboAttackNum)
			{
			case 0:
				PlayAnimMontage(meleeAttackAnimMontage01, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;
				break;
			case 1:
				MotionWarpingMouseCursor();
				PlayAnimMontage(meleeAttackAnimMontage02, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;
				break;
			case 2:
				MotionWarpingMouseCursor();
				PlayAnimMontage(meleeAttackAnimMontage03, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;

				bHasDelayAttack = true;
				break;
			default:
				comboAttackNum = 0;
			}
			skillName = "Normal";
			FTimerHandle th_AttackEnd;
			
			GetWorldTimerManager().SetTimer(th_ComboReset, this, &ASwordCharacter::resetComboAttack, 1.0f, false);
			animLookMouse();
		}
	}
	

	if (bForceMode)
	{
		if (bHasDelayAttack == false)
		{
			switch (comboAttackNum)
			{
			case 0:
				PlayAnimMontage(forceMttackAnimMontage01, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;
				break;
			case 1:
				MotionWarpingMouseCursor();
				PlayAnimMontage(forceMttackAnimMontage02, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;
				break;
			case 2:
				MotionWarpingMouseCursor();
				PlayAnimMontage(forceMttackAnimMontage03, 1.0f);
				isDuringAttack = true;
				comboAttackNum++;

				bHasDelayAttack = true;
				break;
			default:
				comboAttackNum = 0;
			}
			skillName = "NormalF";
			FTimerHandle th_AttackEnd;
			GetWorldTimerManager().SetTimer(th_ComboReset, this, &ASwordCharacter::resetComboAttack, 0.8f, false);
			
			animLookMouse();
		}
	}
}

void ASwordCharacter::animLookMouse()
{
	APlayerController* playerController = Cast<ASwordPlayController>(GetController());
	if (!playerController)
	{
		return;
	}
	

}
void ASwordCharacter::backToOriginMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

void ASwordCharacter::changeRunMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}



void ASwordCharacter::resetComboAttack()
{
	comboAttackNum = 0;
	bHasDelayAttack = true;
	GetWorldTimerManager().SetTimer(th_BasicAttackDelay, this, &ASwordCharacter::delayAttack, 0.3f, false);
}
void ASwordCharacter::delayAttack()
{
	bHasDelayAttack = false;
}

void ASwordCharacter::updateUI()
{
	if (!characterWidget)
	{
		return;
	}
	if (characterWidget)
	{
		float hpPercent = playerCurrentHp / playerMaxHp;
		float mpPercent = playerCurrentMp / playerMaxMp;
		float forcePercent = playerCurrentForce / playerMaxForce;

		playerCurrentForce = FMath::Clamp(playerCurrentForce, 0.0f, 100.0f);

		if (characterWidget->ProgressBar_Hp)
		{
			characterWidget->ProgressBar_Hp->SetPercent(hpPercent);
		}
		if (characterWidget->ProgressBar_Mp)
		{
			characterWidget->ProgressBar_Mp->SetPercent(mpPercent);
		} 
		if (characterWidget->ProgressBar_Force)
		{
			characterWidget->ProgressBar_Force->SetPercent(forcePercent);
		}
	}
}

void ASwordCharacter::StopAOverHeight()
{
	if (bSkillAActive)
	{
		currentZ = GetActorLocation().Z;
		if (currentZ - jumpStartLoc.Z >= 400.f)
		{
			GetCharacterMovement()->DisableMovement();
			bSkillAActive = false;

			
	
			
		}
	}
}




void ASwordCharacter::CheckInAirArea()
{	
	currentLoc = GetActorLocation();
	if (currentLoc.Z >= jumpStartLoc.Z + 300)
	{
		GetCharacterMovement()->StopMovementImmediately();
	}	
}





void ASwordCharacter::playSkillQ()
{

	if (!bCanUseSkillQ)
	{
		return;
	}
	bSSkillMoving = false;
	//swordActorInstance
	if (!bForceMode)
	{
		if (playerCurrentMp >= 100)
		{
			playerCurrentMp -= 200;
			MotionWarpingMouseCursor();
			PlayAnimMontage(qSkillAnim);
			updateUI();

			bCanUseSkillQ = false;
			currentSkillQTime = 0.0f;
			skillName = "Q";

		}
	}
	else
	{
		if (playerCurrentMp >= 100)
		{
			playerCurrentMp -= 200;
			MotionWarpingMouseCursor();
			PlayAnimMontage(ForceQSkillAnim);
			updateUI();

			bCanUseSkillQ = false;
			currentSkillQTime = 0.0f;
			skillName = "QF";
		
		}
	}
}
void ASwordCharacter::playSkillW()
{
	if (!bCanUseSkillW)
	{
		return;
	}
	bSSkillMoving = false;
	if (!bForceMode)
	{
		if (playerCurrentMp >= 200)
		{
			playerCurrentMp -= 400;
			MotionWarpingMouseCursor();
			PlayAnimMontage(wSkillAnim);
			updateUI();

			bCanUseSkillW = false;
			currentSkillWTime = 0.0f;

			skillName = "W";
		
		}
	}
	else
	{
		if (playerCurrentMp >= 400)
		{
			playerCurrentMp -= 200;
			MotionWarpingMouseCursor();
			PlayAnimMontage(ForceWSkillAnim);
			updateUI();

			bCanUseSkillW = false;
			currentSkillWTime = 0.0f;

			skillName = "WF";
	
		}
	}
	
}

void ASwordCharacter::playSkillE()
{
	if (!bCanUseSkillE&& bIsLockKey)
	{
		return;
	}
	bSSkillMoving = false;
	GetCharacterMovement()->GravityScale = 1.0f;
	if (!bForceMode)
	{
		if (playerCurrentMp >= 450)
		{
			playerCurrentMp -= 250;
			MotionWarpingMouseCursor();
			PlayAnimMontage(eSkillAnim);
			updateUI();

			bCanUseSkillE = false;
			currentSkillETime = 0.0f;
			bIsLockKey = true;
			GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 2.0f, false);

			skillName = "E";
	
		}
	}
	else
	{
		if (playerCurrentMp >= 250)
		{
			playerCurrentMp -= 450;
			MotionWarpingMouseCursor();
			PlayAnimMontage(ForceESkillAnim);
			updateUI();

			bCanUseSkillE = false;
			currentSkillETime = 0.0f;
			bIsLockKey = true;
			GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 2.0f, false);

			skillName = "EF";

		}
	}
}

void ASwordCharacter::SlashHitCameraShake()
{
	if (slashActorHitCameraShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(slashActorHitCameraShake);
		}
	}
}


void ASwordCharacter::playSkillR()
{
	//|| !bInCircle
	if (!bCanUseSkillR || bIsLockKey || bRTooHigh )
	{
		return;
	}
	bSSkillMoving = false;
	if (!bForceMode)
	{
		
		if (playerCurrentMp >= 370)
		{
			FHitResult rHitResult = swordPlayerController->skillMouseCursorHit;
			rTargetLoc = rHitResult.ImpactPoint;

			chLoc = GetActorLocation();

			FVector launchDirec = (rTargetLoc - chLoc).GetSafeNormal();

			//calc endLocation
			float launchForce = (rTargetLoc - chLoc).Size();

			//calc gravity
			const float gravity = GetCharacterMovement()->GravityScale * 980.0f;
			const float timeToTarget = FMath::Sqrt((2.0f * launchForce) / gravity);
			float reCalculateZVelocity = (rTargetLoc.Z - chLoc.Z) / timeToTarget + (0.5f * gravity * timeToTarget);
			FVector launchVelocity = launchDirec * launchForce + FVector(0, 0, reCalculateZVelocity);

			GetCharacterMovement()->StopMovementImmediately();
			bIsLockKey = true;
			LaunchCharacter(launchVelocity, true, true);
			FRotator changeForwardLookRotator = launchDirec.Rotation();
			SetActorRotation(changeForwardLookRotator);

			playerCurrentMp -= 170;
			MotionWarpingMouseCursor();
			PlayAnimMontage(rSkillAnim);
			GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 1.3f, false);
			updateUI();
			bCanUseSkillR = false;
			currentSkillRTime = 0.0f;

			skillName = "R";

			
		}
	}
	else
	{	
		if (playerCurrentMp >= 170)
		{
			FHitResult rHitResult = swordPlayerController->skillMouseCursorHit;
			rTargetLoc = rHitResult.ImpactPoint;

			chLoc = GetActorLocation();
			
			FVector launchDirec = (rTargetLoc - chLoc).GetSafeNormal();
			
			//calc endLocation
			float launchForce = (rTargetLoc - chLoc).Size();

			//calc gravity
			const float gravity = GetCharacterMovement()->GravityScale * 980.0f;
			const float timeToTarget = FMath::Sqrt((2.0f * launchForce) / gravity);
			float reCalculateZVelocity = (rTargetLoc.Z - chLoc.Z) / timeToTarget + (0.5f * gravity * timeToTarget);
			FVector launchVelocity = launchDirec * launchForce + FVector(0, 0, reCalculateZVelocity);
			
			GetCharacterMovement()->StopMovementImmediately();
			bIsLockKey = true;
			LaunchCharacter(launchVelocity, true, true);
			FRotator changeForwardLookRotator = launchDirec.Rotation();
			SetActorRotation(changeForwardLookRotator);

			playerCurrentMp -= 370;
			MotionWarpingMouseCursor();
			PlayAnimMontage(ForceRSkillAnim);
			GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 1.3f, false);
			updateUI();
			bCanUseSkillR = false;
			currentSkillRTime = 0.0f;
			//characterWidget->updateRCountDown(7.0f);
			

			skillName = "RF";
		}
	}
	
}

//a
void ASwordCharacter::playSkillA()
{
	
	if (!bCanUseNextSkillA || !bCanUseSkillA &&!bIsLockKey)
	{
		return;
	}
	bSSkillMoving = false;
	if (skillACount == 0)
	{
		bIsLockKey = true;
		playerCurrentMp -= 500;
		GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 6.0f, false);
		bAOESpawnTime = true;
		animInstance = GetMesh()->GetAnimInstance();

		PlayAnimMontage(aSkillStartAnim);
		FOnMontageBlendingOutStarted blendOutDelegate;
		blendOutDelegate.BindUObject(this, &ASwordCharacter::OnSkillStartAnimEnded);
		animInstance->Montage_SetBlendingOutDelegate(blendOutDelegate, aSkillStartAnim);

		FVector launchVelocity = FVector(0.f, 0.f, 1500.f);
		LaunchCharacter(launchVelocity, true, true);

		GetCharacterMovement()->GravityScale = 0.0f;

		aSkillStartCurrentLoc = GetActorLocation();
		jumpStartLoc = aSkillStartCurrentLoc;
		
		bSkillAActive = true;
		skillACount++;
		//widget Cooltime
		if (characterWidget)
		{
			characterWidget->StartProgressBarCoolDown(6.0f);
			characterWidget->ProgressBar_ASkill->SetVisibility(ESlateVisibility::Visible);
		}
		if (gameMode && gameMode->characterWidget)
		{
			gameMode->characterWidget->StartProgressBarCoolDown(6.0f);

			if (gameMode->characterWidget->ProgressBar_ASkill)
			{
				gameMode->characterWidget->ProgressBar_ASkill->SetVisibility(ESlateVisibility::Visible);
			}
		}

		//snap shot
		skillACountSnapshot = skillACount;

		GetWorld()->GetTimerManager().ClearTimer(th_ASkillCheckTimer);
		GetWorld()->GetTimerManager().SetTimer(th_ASkillCheckTimer, this, &ASwordCharacter::checkSkillSnapshot, 6.0f, false);
		
	}
	else if (bHovering && skillACount == 1)
	{
		playerCurrentMp -= 120;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		MotionWarpingMouseCursor();
		animInstance->Montage_Play(aSkillAimingAttack1, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		FOnMontageBlendingOutStarted blendOutDelegate;
		blendOutDelegate.BindUObject(this, &ASwordCharacter::OnSkillStartAnimEnded);
		animInstance->Montage_SetBlendingOutDelegate(blendOutDelegate, aSkillAimingAttack1);
		AAimingSkillCoolTimeSet();
		skillACount++;  

		//snap shot
		skillACountSnapshot = skillACount;

		
	}
	else if (bHovering && skillACount == 2)
	{
		playerCurrentMp -= 120;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		MotionWarpingMouseCursor();
		animInstance->Montage_Play(aSkillAimingAttack2, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		FOnMontageBlendingOutStarted blendOutDelegate;
		blendOutDelegate.BindUObject(this, &ASwordCharacter::OnSkillStartAnimEnded);
		animInstance->Montage_SetBlendingOutDelegate(blendOutDelegate, aSkillAimingAttack2);
		AAimingSkillCoolTimeSet();
		skillACount++;

		//snap shot
		skillACountSnapshot = skillACount;

		
	}
	else if (bHovering && skillACount == 3)
	{
		playerCurrentMp -= 180;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		MotionWarpingMouseCursor();
		animInstance->Montage_Play(aSkillAimingAttack1, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		FOnMontageBlendingOutStarted blendOutDelegate;
		blendOutDelegate.BindUObject(this, &ASwordCharacter::OnSkillEndSkillA);
		animInstance->Montage_SetBlendingOutDelegate(blendOutDelegate, aSkillAimingAttack1);

		bAOESpawnTime = false;
		bCanUseSkillA = false;
		skillACount = 0;
		jumpStartLoc.Z = 0.0f;
	}
}

//sskill
void ASwordCharacter::playSkillS()
{
	if (!IsValid(this))
	{
		return;
	}
	if (characterWidget->innerStack >= 1  || (gameMode && gameMode->characterWidget && gameMode->characterWidget->innerStack >= 1))
	{
		playerCurrentMp -= 150;
		UGameplayStatics::PlaySoundAtLocation(this, sSkillSound, GetActorLocation());
		bIsLockKey = true;
		MotionWarpingMouseCursor();
		PlayAnimMontage(sSkillAnim);
		skillName = "S";
		if (characterWidget)
		{
			characterWidget->UpdateSOutterDownStack();
		}
		if (gameMode)
		{
			gameMode->characterWidget->UpdateSOutterDownStack();
		}
		GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 0.4f, false);
	}	
}

void ASwordCharacter::SkillSMoving(float DeltaTime)
{
	FVector currentSLoc = GetActorLocation();
	FVector targetLoc = SSkilltargetLoc;
	targetLoc.Z = currentSLoc.Z;

	FVector skillSNewLoc = FMath::VInterpTo(currentSLoc, targetLoc, DeltaTime, sSkillMoveSpeed);

	FVector moveVector = skillSNewLoc - currentSLoc;
	moveVector.Z = 0.f;

	FHitResult hit;
	AddActorWorldOffset(moveVector, true, &hit);
	if (FVector::Dist2D(skillSNewLoc, targetLoc) < 1.0f || hit.bBlockingHit)
	{
		bSSkillMoving = false;
	}

}

void ASwordCharacter::StartSSkillMove()
{
	SSkilltargetLoc = GetActorLocation() + GetActorForwardVector() * 800.0f;
	bSSkillMoving = true;
}



void ASwordCharacter::spawnSkillSSlashNiagara()
{
	FVector sSlashNiagaraLoc = GetActorLocation() + FVector(0, 0, 30) + GetActorForwardVector() * 300;
	FRotator sSlashNiagaraRot = GetActorRotation();

	UNiagaraComponent* slashNiaComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), sSlashNiagara, sSlashNiagaraLoc, sSlashNiagaraRot);
	{
		slashNiaComp->SetWorldScale3D(FVector(1.0f));
	}
}

void ASwordCharacter::checkSkillSnapshot()
{
	if (skillACountSnapshot == skillACount)
	{
		OnSkillEndSkillA(nullptr, false);
		
	}
}

void ASwordCharacter::SpawnSwordSlash()
{
	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotation = GetActorRotation();
	FTransform slashSpawnTransform(spawnRotation, spawnLocation);
	AActor* spawnInstance = GetWorld()->SpawnActor<AActor>(slashActor, slashSpawnTransform);
	if (spawnInstance)
	{
		spawnInstance->SetLifeSpan(3.0f);
	}
}

void ASwordCharacter::HoveringCheck()
{

	FVector currentLocation = GetActorLocation();
	if (FMath::Abs(jumpStartLoc.Z) > 10.0f)
	{
		bHovering = true;
	}
	else
	{
		bHovering = false;
	}

}
void ASwordCharacter::OnSkillStartAnimEnded(UAnimMontage* montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->Montage_Play(aSkillAiming, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
}
void ASwordCharacter::OnSkillEndSkillA(UAnimMontage* montage, bool bInterrupted)
{
	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
	GetMesh()->GetAnimInstance()->Montage_Play(aSkillEndAttack, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	GetWorld()->GetTimerManager().SetTimer(th_aAimingAttackEnd, this, &ASwordCharacter::AAimingSkillEndLaunch, 0.45f, false);
	bCanUseSkillA = false;
	currentSkillATime = 0.0f;
	if (characterWidget)
	{
		characterWidget->updateACountDown(25.0f);
	}
	if (gameMode)
	{
		gameMode->characterWidget->updateACountDown(25.0f);
	}

	
	if (characterWidget)
	{
		characterWidget->ProgressBar_ASkill->SetVisibility(ESlateVisibility::Hidden);
	}
	if (gameMode && gameMode->characterWidget && gameMode->characterWidget->ProgressBar_ASkill)
	{
		gameMode->characterWidget->ProgressBar_ASkill->SetVisibility(ESlateVisibility::Hidden);
	}
	bAOESpawnTime = false;
	bCanUseSkillA = false;
	skillACount = 0;
	jumpStartLoc.Z = 0.0f;
	
}
void ASwordCharacter::AAimingSkillCoolTimeSet()
{
	bCanUseNextSkillA = false;
	GetWorld()->GetTimerManager().SetTimer(th_aAimingAttackCooltime, this, &ASwordCharacter::AAimingSkillCoolTimeReSet, 0.8f, false);
}
void ASwordCharacter::AAimingSkillCoolTimeReSet()
{
	bCanUseNextSkillA = true;
	
}
void ASwordCharacter::AAimingSkillEndLaunch()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	FVector launchVelocity = FVector(0.f, 0.f, -3000.f);
	LaunchCharacter(launchVelocity, true, true);
	
	
}
void ASwordCharacter::playSkillAEnd()
{
	
	
	
}

void ASwordCharacter::playSkillD()
{

}
void ASwordCharacter::playSkillF()
{
	playerCurrentForce = 100;
	updateUI();

	
	
}

void ASwordCharacter::unlockKey()
{
	bIsLockKey = false;

}

void ASwordCharacter::playForceAwake()
{
	if (playerCurrentForce == 100)
	{
		bForceMode = true;
		GetCharacterMovement()->StopMovementImmediately();
		forceAwakeParticle->SetActive(true);
		PlayAnimMontage(forceAwakeAnimMontage);
		bIsLockKey = true;
		GetWorldTimerManager().SetTimer(th_UnlockSkill, this, &ASwordCharacter::unlockKey, 0.8f, false);
		GetWorldTimerManager().SetTimer(th_AwakeForce, this, &ASwordCharacter::forceAwakeAfter, 0.5f, false);
		decreaseForceAfterUseForce();
		//playerCurrentForce = 0;
		updateUI();
		if (swordActorInstance && swordActorInstance->swordMesh)
		{
			swordActorInstance->swordMesh->SetMaterial(0, forceSwordMaterial);
		}
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		if (characterWidget)
		{
			characterWidget->LockDownASkillActive();
		}
		if (gameMode->characterWidget)
		{
			gameMode->characterWidget->LockDownASkillActive();
		}
		
	}

}
void ASwordCharacter::decreaseForceAfterUseForce()
{
	float decreaseDuration = 50.0f; // 폭주시간
	float tickInterval = 0.01f; // 0.1초마다 감소
	forceDecreaseAmount = playerMaxForce / (decreaseDuration / tickInterval);

	GetWorldTimerManager().SetTimer(th_decreaseForce, this, &ASwordCharacter::decreaseForceTick, tickInterval, true);

}

void ASwordCharacter::decreaseForceTick()
{
	if (playerCurrentForce > 0)
	{
		playerCurrentForce -= forceDecreaseAmount;
		if (playerCurrentForce < 0)
		{
			playerCurrentForce = 0;
			GetWorldTimerManager().ClearTimer(th_decreaseForce);
		}

		updateUI();
	}
	if (playerCurrentForce == 0)
	{
		forceAwakeEnd();
		characterWidget->StopForce();
		
	}


}


void ASwordCharacter::forceAwakeAfter()
{
	forceAwakeLightning->SetVisibility(true);
	forceBodyLightning->SetVisibility(true);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), awakeLightningStrike, GetActorLocation());
		playerController->ClientStartCameraShake(forceCameraShake);
		UGameplayStatics::PlaySound2D(this, awakeSound);
	}

}

void ASwordCharacter::forceAwakeEnd()
{
	forceAwakeLightning->SetVisibility(false);
	forceBodyLightning->SetVisibility(false);
	if (swordActorInstance && swordActorInstance->swordMesh)
	{
		swordActorInstance->swordMesh->SetMaterial(0, basicSwordMaterial);
	}
	//basicSwordMesh->SetMaterial(0, basicSwordMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bForceMode = false; 
	if (characterWidget)
	{
		characterWidget->LockDownASkillNonActive();
	}
	if (gameMode->characterWidget)
	{
		gameMode->characterWidget->LockDownASkillNonActive();
	}
	
}

void ASwordCharacter::cameraWZoomIn()
{
	if (forceMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;
				blendables.Add(FWeightedBlendable(1.0f, forceMaterialInstance));
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(th_zoomInTimer, this, &ASwordCharacter::zoomInStep, 0.001f, true);
	
}
void ASwordCharacter::cameraWZoomOut()
{
	GetWorld()->GetTimerManager().SetTimer(th_zoomOutTimer, this, &ASwordCharacter::zoomOutStep, 0.016f, true);
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		playerController->ClientStartCameraShake(forceWCameraShake);
	}
	if (forceMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;
				for (int32 i = blendables.Num() - 1; i >= 0; --i)
				{
					if (blendables[i].Object == forceMaterialInstance)
					{
						blendables.RemoveAt(i);
					}
				}
			}
		}
	}
	
}

void ASwordCharacter::StartRadialBlur()
{
	if (radialBlurMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				//blending list setting!
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;

				// if insert no add
				bool alreadyAdded = false;
				for (const FWeightedBlendable& blendable : blendables)
				{
					if (blendable.Object == radialBlurMaterialInstance)
					{
						alreadyAdded = true;
						break;
					}
				}
				if (!alreadyAdded)
				{
					if (blendables.Num() >= 1)
					{
						blendables.Insert(FWeightedBlendable(1.0f, radialBlurMaterialInstance), 1);
					}
					else
					{
						blendables.Add(FWeightedBlendable(1.0f, radialBlurMaterialInstance));
					}
				}
			}
		}
	}
}

void ASwordCharacter::ShowGhostTrail()
{
	ghostTrailComponent->Activate(true);
}

void ASwordCharacter::HideGhostTrail()
{
	ghostTrailComponent->Deactivate();
}



void ASwordCharacter::EndRadialBlur()
{
	if (radialBlurMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;

				// remove!
				for (int32 i = blendables.Num() - 1; i >= 0; --i)
				{
					if (blendables[i].Object == radialBlurMaterialInstance)
					{
						blendables.RemoveAt(i);
					}
				}
			}
		}
	}
}
void ASwordCharacter::cameraFarAway()
{
	GetWorld()->GetTimerManager().SetTimer(th_zoomFarAwayTimer, this, &ASwordCharacter::zoomFarAwayStep, 0.008f, true);
}
void ASwordCharacter::cameraSetOrigin()
{
	GetWorld()->GetTimerManager().SetTimer(th_zoomOutTimer, this, &ASwordCharacter::zoomOutStep, 0.018f, true);
}
void ASwordCharacter::zoomInStep()
{
	//not yet in
	if (!FMath::IsNearlyEqual(CameraComponent->FieldOfView, zoomFov, 0.1f)) 
	{
		float nextZoomFov = FMath::FInterpTo(CameraComponent->FieldOfView, zoomFov, 0.03f, zoomSpeed);
		CameraComponent->SetFieldOfView(nextZoomFov);
	}
	else
	{
		CameraComponent->SetFieldOfView(zoomFov);
		GetWorld()->GetTimerManager().ClearTimer(th_zoomInTimer);
	}
}

void ASwordCharacter::zoomOutStep()
{
	if (!FMath::IsNearlyEqual(CameraComponent->FieldOfView, 90.0f, 0.1f))
	{
		float nextOutFov = FMath::FInterpTo(CameraComponent->FieldOfView, 90.0f, 0.02f, zoomSpeed * 2.0f);
		CameraComponent->SetFieldOfView(nextOutFov);
	}
	else
	{
		CameraComponent->SetFieldOfView(90.0f); 
		GetWorld()->GetTimerManager().ClearTimer(th_zoomOutTimer);
	}
}

void ASwordCharacter::zoomFarAwayStep()
{
	if (!FMath::IsNearlyEqual(CameraComponent->FieldOfView, outFov, 0.1f))
	{
		float nextZoomFov = FMath::FInterpTo(CameraComponent->FieldOfView, outFov, 0.03f, zoomSpeed);
		CameraComponent->SetFieldOfView(nextZoomFov);
	}
	else
	{
		CameraComponent->SetFieldOfView(outFov);
		GetWorld()->GetTimerManager().ClearTimer(th_zoomFarAwayTimer);
	}
}

void ASwordCharacter::spawnSkillIndicatorDecal()
{
	if (!spawnDecal)
	{
		FHitResult spawnLoc = swordPlayerController->skillMouseCursorHit;
		FVector hitLocation = spawnLoc.ImpactPoint;
		FRotator hitRotation = FRotator::ZeroRotator;
		spawnDecal = GetWorld()->SpawnActor<AIndicatorDecalActor>(skillIndicatorDecal, hitLocation, hitRotation);
		
		spawnDecal->SetActorScale3D(FVector(1.0f));
		
		
	}

}
void ASwordCharacter::spawnSkillImpactDecal(float size)
{
	
	//if (spawnImpactDecal)
	//{
		
		FHitResult spawnLoc = swordPlayerController->skillMouseCursorHit;
		FVector hitLocation = spawnLoc.ImpactPoint;
		FRotator hitRotation = FRotator::ZeroRotator;

		spawnImpactDecal = GetWorld()->SpawnActor<AIndicatorDecalActor>(
			skillImpactIdicator,
			hitLocation,
			hitRotation
		);
	//}
	

	//if (spawnImpactDecal)
	//{
		spawnImpactDecal->SetActorScale3D(FVector(size));
	//}
	
		
}

//skill a under decal
void ASwordCharacter::spawnUnderSkillImpactDecal(float size)
{
	FVector playerLoc = GetActorLocation() - FVector(0, 0, -400);
	FRotator playerRot = GetActorRotation();

	spawnAoeR = GetWorld()->SpawnActor<AAoeCircleDecal>(
		skillAOEDecal,
		playerLoc,
		playerRot
	);
 
	if (spawnAoeR)
	{
		spawnAoeR->SetActorScale3D(FVector(size));
	}
	

}

void ASwordCharacter::spawnSkillIndicatorDecalAVer()
{
	if (!spawnDecal)
	{
	
		FHitResult spawnLoc = swordPlayerController->skillMouseCursorHit;
		FVector hitLocation = spawnLoc.ImpactPoint;
		FRotator hitRotation = FRotator::ZeroRotator;

		spawnDecal = GetWorld()->SpawnActor<AIndicatorDecalActor>(skillIndicatorDecal, hitLocation, hitRotation);
		spawnDecal->SetActorScale3D(FVector(0.9f));
	}
}
void ASwordCharacter::DestroySkillIndicatorDecal()
{
	if (spawnDecal)
	{
		spawnDecal->Destroy();  
		spawnDecal = nullptr;   
	}
}

void ASwordCharacter::DestroyImpactIndicatorDecal()
{
	if (spawnImpactDecal)
	{
		spawnImpactDecal->Destroy();
		spawnImpactDecal = nullptr;
	}
}

void ASwordCharacter::spawnAOEDecal(float spawnSize)
{
	spawnAoeR = GetWorld()->SpawnActor<AAoeCircleDecal>(
		skillAOEDecal,
		swordPlayerController->currentPlayerLoc,
		swordPlayerController->currentPlayerRot
	);

	if (spawnAoeR)
	{
		spawnAoeR->SetActorScale3D(FVector(spawnSize));
	}

}
void ASwordCharacter::DestroyAOEDecal()
{
	if (spawnAoeR)
	{
		spawnAoeR->Destroy();
		spawnAoeR = nullptr;
	}
}
void ASwordCharacter::checkInAOEDecalRange()
{
	if (!spawnAoeR)
	{
		return;
	}
	FVector aoe_Loc = spawnAoeR->GetActorLocation();
	FVector cursor_Loc = swordPlayerController->skillMouseCursorHit.ImpactPoint;

	//aoeRadius = 558.0f;
	float mouseToAoeCenterDist = FVector::Dist(cursor_Loc, aoe_Loc);
	
	if (mouseToAoeCenterDist <= aoeRadius)
	{
		//red -> blue (mousecursor in circle)
		//spawnImpactDecal->setMPCScalar0Value();
		bInCircle = true;
	}
	else
	{
		//blue -> red (mousecursor out circle)
		//spawnImpactDecal->setMPCScalar1Value();
		bInCircle = false;
	}

}






void ASwordCharacter::spawnRLastSlash()
{
	if (forceRLastSkillNia)
	{
		FVector spawnLoc = GetActorLocation() + FVector(50, 0, 0);
		FRotator spawnRot = GetActorRotation() + FRotator(0, 180, 0);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), forceRLastSkillNia, spawnLoc, spawnRot, FVector(1.6f));
	}
}
void ASwordCharacter::cameraShakeR()
{
	if (forceRCameraShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(forceRCameraShake);
		}
	}
}

void ASwordCharacter::cameraShakeQ()
{
	if (forceQCameraShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(forceQCameraShake);
		}
	}
}

void ASwordCharacter::spawnOriginGreatSword()
{
	if (originGreatSword)
	{
		FVector spawnLoc = GetActorLocation() + FVector(0, 0, -60) +GetActorForwardVector() * 100.0f;
		FRotator spawnRot = GetActorRotation();

		AActor* spawnGreatSword = GetWorld()->SpawnActor<AActor>(originGreatSword, spawnLoc, spawnRot);

		if (spawnGreatSword)
		{
			spawnGreatSword->SetLifeSpan(1.5f);
		}
	}
	
}

void ASwordCharacter::spawnForceGreatSword()
{
	if (forceGreatSword)
	{
		FVector spawnLoc = GetActorLocation() + FVector(0, 0, -60) + GetActorForwardVector() * 100.0f;
		FRotator spawnRot = GetActorRotation();

		AActor* spawnGreatSword = GetWorld()->SpawnActor<AActor>(forceGreatSword, spawnLoc, spawnRot);

		if (spawnGreatSword)
		{
			spawnGreatSword->SetLifeSpan(1.5f);
		}
	}

}
void ASwordCharacter::CameraShakeEGreatSwordNormal()
{
	if (originEGreatSwordShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(originEGreatSwordShake);
		}
	}
	if (forceRLastSkillNia)
	{
		FVector spawnLoc = GetActorLocation() + FVector(0, 0, -60) + GetActorForwardVector() * 500.0f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), eNormalImpactParticle, spawnLoc, FRotator::ZeroRotator, FVector(0.5f));
		
	}
}
void ASwordCharacter::CameraShakeEGreatSwordForce()
{
	if (forceEGreatSwordShake)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			playerController->ClientStartCameraShake(forceEGreatSwordShake);
		}
	}
	if (forceRLastSkillNia)
	{
		FVector spawnLoc = GetActorLocation() + FVector(0, 0, -60) + GetActorForwardVector() * 500.0f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), eForceImpactParticle, spawnLoc, FRotator::ZeroRotator, FVector(1.5f));

	}
}



void ASwordCharacter::getDamageDatableValue(float damage)
{
	if (statGI)
	{
		normalAttackDamage = statGI->normalAttackDamage;
		normalLastAttackDamage = statGI->normalLastAttackDamage;
		qAttackDamage = statGI->qAttackDamage;
		qAttackLastDamage = statGI->qAttackLastDamage;
		wAttackDamage = statGI->wAttackDamage;
		wAttackLastDamage = statGI->wAttackLastDamage;
		eAttackDamage = statGI->eAttackDamage;
		eAttackLastDamage = statGI->eAttackLastDamage;
		rAttackDamage = statGI->rAttackDamage;
		rAttackLastDamage = statGI->rAttackLastDamage;

		
	}
}

void ASwordCharacter::ClearSkillName()
{
	swordActorInstance->ClearSkillName();

}

void ASwordCharacter::MotionWarpingMouseCursor()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController)
	{
		return;
	}

	FVector worldLocation, worldDirection;
	if (playerController->DeprojectMousePositionToWorld(worldLocation, worldDirection))
	{
		FVector start = worldLocation;
		FVector end = start + (worldDirection * 10000.0f);

		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params))
		{
			FVector targetLocation = hitResult.Location;

			
			FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();
			FRotator targetRotation = direction.Rotation();

			// calculate targetLoc
			FVector desiredLocation = GetActorLocation() + direction * 100.0f;

			// sphereTrace
			FHitResult moveHit;
			float sphereRadius = 25.0f;

			bool bHit = GetWorld()->SweepSingleByChannel(moveHit,GetActorLocation(),desiredLocation,FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(sphereRadius),
				params
			);

			FVector finalLocation;
			if (bHit)
			{
				finalLocation = GetActorLocation();
			}
			else
			{
				finalLocation = desiredLocation;
			}

			// final transform
			targetRotation.Pitch = 0.0f;
			targetRotation.Roll = 0.0f;

			FTransform targetTransform;
			targetTransform.SetLocation(finalLocation);
			targetTransform.SetRotation(targetRotation.Quaternion());
			

			if (motionWarpComponent)
			{
				motionWarpComponent->AddOrUpdateWarpTargetFromTransform(TEXT("Target"), targetTransform);
			}

			//DrawDebugSphere(GetWorld(), finalLocation, 15.0f, 12, FColor::Cyan, false, 2.0f);
		
		}
	}

}



void ASwordCharacter::SpawnIgnoreKnockBackActor()
{
	AActor* spawnInstance = GetWorld()->SpawnActor<AActor>(IgnoreKnockbackActor, GetActorLocation(), GetActorRotation());
	if (spawnInstance)
	{
		spawnInstance->SetActorScale3D(FVector(4.0f));
		spawnInstance->SetLifeSpan(0.2f);
	}
}

void ASwordCharacter::UpdateCameraArmLength(float DeltaTime)
{
	float detectArmLength = SpringArmComponent->TargetArmLength;
	//min
	float minArmDesiredLength = FMath::Max(originArmLength, minArmLength);
	float dynamicInterpSpeed = armLengthChangeSpeed * DeltaTime;
	currentArmLength = FMath::FInterpTo(currentArmLength, minArmDesiredLength, DeltaTime, dynamicInterpSpeed);
	SpringArmComponent->TargetArmLength = currentArmLength;

}


//real damage state
void ASwordCharacter::GetDamage(float damage)
{
	
	playerCurrentHp -= damage;
	playerCurrentHp = FMath::Clamp(playerCurrentHp, 0.0f, playerMaxHp);
	if (IsValid(characterWidget))
	{
		characterWidget->updateHpBar(playerCurrentHp, playerMaxHp);
	}
	
	if (myInstance)
	{
		myInstance->setDungeonSkeletonAttackCounting();
	}
	updateUI();
	if (playerCurrentHp <= 0)
	{
		if (ASwordPlayController* playerCon = Cast<ASwordPlayController>(GetController()))
		{
			playerCon->RevivePlayer(this); 
		}
	}
}

void ASwordCharacter::playSwitchAnim()
{
	PlayAnimMontage(switchSwordMontage);
}

void ASwordCharacter::TriggerDeathAndRevive(bool bIsDie)
{
	UAnimInstance* animIns = GetMesh()->GetAnimInstance();
	if (!animIns) return;

	if (bIsDie)
	{
		if (dieMontage)
		{
			animIns->Montage_Play(dieMontage);
			TArray<AActor*> FoundMonsters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicSkeletonEnemy::StaticClass(), FoundMonsters);

			for (AActor* Actor : FoundMonsters)
			{
				APawn* monsterPawn = Cast<APawn>(Actor);
				if (!monsterPawn) continue;

				ASkeletonAIController* skeletonCon = Cast<ASkeletonAIController>(monsterPawn->GetController());
				if (skeletonCon)
				{
					skeletonCon->DisablePerception();
				}
			}
			GetWorldTimerManager().SetTimer(th_DieMontageEnd, this, &ASwordCharacter::OnDieMontageEnded, 3.5f, false);
		}
	}
	else
	{
		
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		TArray<AActor*> FoundMonsters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicSkeletonEnemy::StaticClass(), FoundMonsters);

		for (AActor* Actor : FoundMonsters)
		{
			APawn* monsterPawn = Cast<APawn>(Actor);
			if (!monsterPawn) continue;

			ASkeletonAIController* skeletonCon = Cast<ASkeletonAIController>(monsterPawn->GetController());
			if (skeletonCon)
			{
				skeletonCon->EnablePerception();
			}
		}
		//Reset HP, MP
		gameInstance = Cast<UStatGameInstance>(GetGameInstance());
		if (gameInstance)
		{
			gameInstance->playerSaveHp = playerMaxHp;
			gameInstance->playerSaveMp = playerMaxMp;
		}
		gameInstance->SavePlayerData(GetWorld());

		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		ASwordPlayController* swordController = Cast<ASwordPlayController>(playerController);
		swordController->InputTab();
		
	}
}

void ASwordCharacter::OnDieMontageEnded()
{
	SetActorHiddenInGame(true);         
}

void ASwordCharacter::SetUpdateGameModeGamePlayWidget()
{
	
	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
	if (gameMode->characterWidget)
	{
		float hpPercent = playerCurrentHp / playerMaxHp;
		float mpPercent = playerCurrentMp / playerMaxMp;
		float forcePercent = playerCurrentForce / playerMaxForce;

		playerCurrentForce = FMath::Clamp(playerCurrentForce, 0.0f, 100.0f);

		characterWidget->ProgressBar_Hp->SetPercent(hpPercent);
		characterWidget->ProgressBar_Mp->SetPercent(mpPercent);
		characterWidget->ProgressBar_Force->SetPercent(forcePercent);
	}
}

void ASwordCharacter::ShowViewPortMainWidget()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController)
	{
		if (characterWidget && characterWidget->IsInViewport())
		{
			characterWidget->GetWorld()->GetTimerManager().ClearTimer(characterWidget->th_InnerProgressTimer);

			characterWidget->RemoveFromParent();
			characterWidget = nullptr;
		}

		characterWidget = CreateWidget<USwordCharacterWidget>(playerController, LoadClass<USwordCharacterWidget>(nullptr, TEXT("/Game/Luco/UI/SwordCharacterWidget.SwordCharacterWidget_C")));

		//if (characterWidget && gameMode && gameMode->bUseCharactersequence == false)
		if (gameMode && !gameMode->bUseCharactersequence)
		{
			characterWidget->AddToViewport();
			characterWidget->UpdateSInnerCountDown(4.0f);
			
			ASwordPlayController* swordController = Cast<ASwordPlayController>(playerController);
			if (swordController)
			{
				swordController->swordCharacterWidget = characterWidget;
			}
			updateUI();

			gameMode->OnPlayPlayerWidgetReady();
			gameMode->HideAllPlayerUI();
		}
	}
}

void ASwordCharacter::ShowViewPortMainWidgetGameModeVer()
{
	if (gameMode->bUseCharactersequence == false) return;
	if (gameMode->bUseCharactersequence == true)
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (playerController)
		{
			if (characterWidget && characterWidget->IsInViewport())
			{
				characterWidget->RemoveFromParent();
				characterWidget = nullptr;
			}

			characterWidget = CreateWidget<USwordCharacterWidget>(playerController, LoadClass<USwordCharacterWidget>(nullptr, TEXT("/Game/Luco/UI/SwordCharacterWidget.SwordCharacterWidget_C")));

			if (characterWidget && gameMode && gameMode->bUseCharactersequence)
			{
				characterWidget->AddToViewport();
				characterWidget->UpdateSInnerCountDown(4.0f);

				ASwordPlayController* swordController = Cast<ASwordPlayController>(playerController);
				if (swordController)
				{
					swordController->swordCharacterWidget = characterWidget;
				}
				updateUI();

				gameMode->OnPlayPlayerWidgetReady();
				gameMode->HideAllPlayerUI();
			}
		}
	}
	
}

// --- 아이템 사용했을때 어떤 아이템을 사용했는지 확인 용도

void ASwordCharacter::usingItems(const FString& UsingItemName)
{
	FString copyName = UsingItemName;
	if (copyName == TEXT("HP_potion"))
	{
		playerCurrentHp += 1500;
		UGameplayStatics::PlaySound2D(this, usePoitionSound);
	}
	else if (copyName == TEXT("MP_potion"))
	{
		playerCurrentMp += 500;
		UGameplayStatics::PlaySound2D(this, usePoitionSound);
	}
	else if (copyName == TEXT("Elixir_potion"))
	{
		playerCurrentHp = playerMaxHp;
		playerCurrentMp = playerMaxMp;
	}
	else if (copyName == TEXT("Force_potion"))
	{
		playerCurrentForce = playerMaxForce;
	}
	else if (copyName == TEXT("Propital_potion"))
	{
		usingP_potion = true;
		RegenRateHP = 0;
	}


	if (playerCurrentMp > playerMaxMp)
		playerCurrentMp = playerMaxMp;
	if (playerCurrentHp > playerMaxHp)
		playerCurrentHp = playerMaxHp;

}

//특수 아이템 (자연 회복을 하게 하는 물약) 했을때
void ASwordCharacter::usingPropital(float DeltaTime)
{
	float HPRegenRate = 200.0f; // 초당 회복량
	playerCurrentHp = FMath::Clamp(playerCurrentHp + HPRegenRate * DeltaTime, 0.0f, playerMaxHp);

	RegenRateHP += HPRegenRate * DeltaTime;
	if (playerCurrentHp > playerMaxHp)
		playerCurrentHp = playerMaxHp;

	if (RegenRateHP >= 2000)
		usingP_potion = false;

}

void ASwordCharacter::SetPauseWidget()
{
	//NoPause
	if (pauseWidget && pauseWidget->IsInViewport())
	{
		pauseWidget->RemoveFromParent();
		pauseWidget = nullptr;
		gameMode->SetBGMMute(false);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); 
	}
	//Pause
	else
	{
		UGameplayStatics::PlaySound2D(this, pauseSound);
		pauseWidget = CreateWidget<UPauseWidget>(swordPlayerController, PauseWidgetClass);
		if (pauseWidget)
		{
			pauseWidget->AddToViewport(10);
			gameMode->SetBGMMute(true);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f); 
		}
	}
}

void ASwordCharacter::FootStep()
{
	//staticmesh()
	FVector startPoint = GetActorLocation();
	FVector endPoint = startPoint - FVector(0.f, 0.f, 150.f);

	FHitResult hit;
	FCollisionQueryParams footStepParams;
	footStepParams.AddIgnoredActor(this);
	footStepParams.bTraceComplex = false;

	bool bHitLineTraceFootStep = GetWorld()->LineTraceSingleByChannel(hit, startPoint, endPoint, ECC_WorldStatic, footStepParams);
	if (bHitLineTraceFootStep)
	{
		EPhysicalSurface surfaceType = SurfaceType_Default;
		UPhysicalMaterial* matFromMaterial = hit.PhysMaterial.Get();
		if (matFromMaterial)
		{
			surfaceType = UPhysicalMaterial::DetermineSurfaceType(matFromMaterial);
		}
		else
		{
			if (hit.Component.IsValid())
			{
				UPhysicalMaterial* overridePhysMat = hit.Component->BodyInstance.GetSimplePhysicalMaterial();
				if (overridePhysMat)
				{
					surfaceType = overridePhysMat->SurfaceType;
				}
			}
		}
		switch (surfaceType)
		{
		case SurfaceType_Default:
			break;
			//NormalTile
		case SurfaceType1:
			if (sound_NormalTile.Num() > 0)
			{
				int32 normalTileIndex = FMath::RandRange(0, sound_NormalTile.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_NormalTile[normalTileIndex], hit.ImpactPoint);
			}
			break;
			//DungeonTile
		case SurfaceType2:
			if (sound_DungeonTile.Num() > 0)
			{
				int32 dungeonTileIndex = FMath::RandRange(0, sound_DungeonTile.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_DungeonTile[dungeonTileIndex], hit.ImpactPoint);
			}
			break;
			//Sand
		case SurfaceType3:
			if (sound_Sand.Num() > 0)
			{
				int32 sandIndex = FMath::RandRange(0, sound_Sand.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Sand[sandIndex], hit.ImpactPoint);
			}
			break;
			//Wood
		case SurfaceType4:
			if (sound_Wood.Num() > 0)
			{
				int32 woodIndex = FMath::RandRange(0, sound_Wood.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Wood[woodIndex], hit.ImpactPoint);
			}
			break;
		case SurfaceType5:
			if (sound_Grass.Num() > 0)
			{
				int32 grassIndex = FMath::RandRange(0, sound_Grass.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Grass[grassIndex], hit.ImpactPoint);
			}
			break;
		case SurfaceType6:
			if (sound_Water.Num() > 0)
			{
				int32 waterIndex = FMath::RandRange(0, sound_Water.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Water[waterIndex], hit.ImpactPoint);
			}
			break;
		case SurfaceType7:
			if (sound_Stone.Num() > 0)
			{
				int32 stoneIndex = FMath::RandRange(0, sound_Stone.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Stone[stoneIndex], hit.ImpactPoint);
			}
			break;
		default:
			break;
		}
	}
	//landscape
	FHitResult randScapeHitResult = {};
	FCollisionQueryParams randScapeParam = {};
	randScapeParam.bReturnPhysicalMaterial = true;
	randScapeParam.AddIgnoredActor(this);
	bool bHitRandScape = GetWorld()->LineTraceSingleByChannel(randScapeHitResult, startPoint, endPoint, ECC_GameTraceChannel6, randScapeParam);
	//loc, rot
	UCapsuleComponent* characterCapsule = GetCapsuleComponent();
	FVector niagaraBottomLoc = characterCapsule->GetComponentLocation() - FVector(0, 0, characterCapsule->GetScaledCapsuleHalfHeight());
	FVector spawnLocation = niagaraBottomLoc + FVector(0, 0, 5.0f);
	FRotator spawnRotation = FRotator::ZeroRotator;


	if (bHitRandScape)
	{
		// PhysMaterial 얻기
		UPhysicalMaterial* physMat = randScapeHitResult.PhysMaterial.Get();
		if (physMat)
		{
			// SurfaceType 가져오기
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(physMat);
			switch (surfaceType)
			{
			case SurfaceType_Default:
				break;
				//NormalTile
			case SurfaceType1:
				if (sound_NormalTile.Num() > 0)
				{
					int32 normalTileIndex = FMath::RandRange(0, sound_NormalTile.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_NormalTile[normalTileIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), normalTileEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
				//DungeonTile
			case SurfaceType2:
				if (sound_DungeonTile.Num() > 0)
				{
					int32 dungeonTileIndex = FMath::RandRange(0, sound_DungeonTile.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_DungeonTile[dungeonTileIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), dungeonTileEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
				//Sand
			case SurfaceType3:
				if (sound_Sand.Num() > 0)
				{
					int32 sandIndex = FMath::RandRange(0, sound_Sand.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Sand[sandIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), sandEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
				//Wood
			case SurfaceType4:
				if (sound_Wood.Num() > 0)
				{
					int32 woodIndex = FMath::RandRange(0, sound_Wood.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Wood[woodIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), woodEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
				//Grass
			case SurfaceType5:
				if (sound_Grass.Num() > 0)
				{
					int32 grassIndex = FMath::RandRange(0, sound_Grass.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Grass[grassIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), grassEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
				//Water
			case SurfaceType6:
				if (sound_Water.Num() > 0)
				{
					int32 waterIndex = FMath::RandRange(0, sound_Water.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Water[waterIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), waterEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
			case SurfaceType7:
				//Stone
				if (sound_Stone.Num() > 0)
				{
					int32 stoneIndex = FMath::RandRange(0, sound_Stone.Num() - 1);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Stone[stoneIndex], hit.ImpactPoint);
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), stoneEffect, spawnLocation, spawnRotation, FVector(2.0f), true, true, ENCPoolMethod::AutoRelease);
				}
				break;
			default:
				break;
			}
		}
	}
}

void ASwordCharacter::Polymorph()
{
	//camera


	GetMesh()->SetVisibility(true);
	overlapSword->SetVisibility(true);
	playerCurrentForce = 0;
	GetWorldTimerManager().ClearTimer(th_decreaseForce);
	forceAwakeEnd();
	//characterWidget->StopForce();
	characterWidget->PlayAnimEndPlySwordReset();
	
	forceAwakeLightning->SetVisibility(false);
	GetWorldTimerManager().SetTimer(th_SpawnSequenceDragon, this, &ASwordCharacter::SpawnSequenceDragon, 1.0667f, false);
	PlayAnimMontage(montageBackStep);
	APlayerController* playCon = Cast<APlayerController>(GetController());
	if (playCon && skillCamera)
	{
		skillCamera->SetActive(true);
		CameraComponent->SetActive(false);
		playCon->DisableInput(playCon);
		playCon->StopMovement();
	}

	if (skillCamera)
	{
		FVector relativeOffset = FVector(-70.f, 230.f, 110.f);
		FVector rotatedOffset = GetActorRotation().RotateVector(relativeOffset);
		FVector cameraWorldLoc = GetActorLocation() + rotatedOffset;
		FRotator cameraWorldRot = (GetActorLocation() - cameraWorldLoc).Rotation();
		skillCamera->SetWorldLocation(cameraWorldLoc);
		skillCamera->SetWorldRotation(cameraWorldRot);
		skillCamera->SetActive(true);
	}
	FMovieSceneSequencePlaybackSettings playSetting;
	ALevelSequenceActor* sequenceActor = nullptr;
	playSetting.bAutoPlay = false;

	swordSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), characterPolyMorphSequence, playSetting, sequenceActor);
	if (swordSequencePlayer)
	{
		swordSequencePlayer->OnFinished.AddDynamic(this, &ASwordCharacter::OnPolymorphSequenceFinished);
		swordSequencePlayer->Play();
	}

	characterWidget->ResetGaze();
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
	{
		UProjectSaveGame* loadedData = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveData"), 0));
		if (loadedData)
		{
			loadedData->saveCurrentSoulGaze = 0.f;
			loadedData->saveBGaze14 = false;

			UGameplayStatics::SaveGameToSlot(loadedData, TEXT("PlayerSaveData"), 0);
		}
	}
	characterWidget->SetSoulCollectionBar(0.f);
	equipmentUI->SetVisibility(ESlateVisibility::Hidden);
	bloodWidget->SetVisibility(ESlateVisibility::Hidden);
	characterWidget->SetVisibility(ESlateVisibility::Hidden);
	
}

//playCon->SetViewTargetWithBlend(this, 0.5f); 
void ASwordCharacter::OnPolymorphSequenceFinished()
{
	APlayerController* playCon = Cast<APlayerController>(GetController());

	if (playCon && skillCamera && CameraComponent)
	{
		equipmentUI->SetVisibility(ESlateVisibility::Visible);
		bloodWidget->SetVisibility(ESlateVisibility::Visible);
		characterWidget->SetVisibility(ESlateVisibility::Visible);
		swordPlayerController->EnableInput(swordPlayerController);
	}
	//잠깐만 주석
	sequenceDragon->Destroy();
	sequenceDragon = nullptr;

	
	//new poccess
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController) return;

	this->SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	this->SetActorTickEnabled(false);
	playerController->UnPossess();

	FVector dragonSpawnLocation = GetActorLocation();
	FRotator dragonSpawnRotation = GetActorRotation();

	//Once Spawn DragonActor
	if (!newDragon)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		newDragon = GetWorld()->SpawnActor<ADragonCharacter>(dragonCharacterClass, dragonSpawnLocation, dragonSpawnRotation, spawnParams);
		newDragon->rememberedSwordCharacter = this;
	}
	
	//pointer set
	rememberedSwordController = playerController;
	//Once Spawn DragonController
	if (newDragon && !bIsDragonSpawned)
	{
		//강제이동 방지
		newDragon->bInterpZ = false;
		if (equipmentUI) equipmentUI->SetVisibility(ESlateVisibility::Hidden);
		if (bloodWidget) bloodWidget->SetVisibility(ESlateVisibility::Hidden);
		if (characterWidget) characterWidget->SetVisibility(ESlateVisibility::Hidden);

		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		newPossessController = GetWorld()->SpawnActor<APlayerController>(dragonControllerClass);
		
		if (newPossessController && localPlayer)
		{
			localPlayer->PlayerController = newPossessController;
			newPossessController->Player = localPlayer;
			newPossessController->Possess(newDragon);

			bIsDragonSpawned = true; 
		}
		newDragon->dragonWidget->StartPercentageOrbCountDown();
	}
	//already spawn controller
	else if (newDragon && bIsDragonSpawned)
	{
		newDragon->bInterpZ = false;
		newDragon->SetActorLocation(dragonSpawnLocation);
		newDragon->SetActorRotation(dragonSpawnRotation);
		newDragon->SetActorHiddenInGame(false);
		newDragon->SetActorEnableCollision(true);
		newDragon->SetActorTickEnabled(true);

		playerController->UnPossess();

		ULocalPlayer* localPlayer = playerController->GetLocalPlayer();
		if (localPlayer)
		{
			localPlayer->PlayerController = newPossessController;
			newPossessController->Player = localPlayer;
			newPossessController->Possess(newDragon);
		}
		// 사람 UI 숨기기
		if (equipmentUI) equipmentUI->SetVisibility(ESlateVisibility::Hidden);
		if (bloodWidget) bloodWidget->SetVisibility(ESlateVisibility::Hidden);
		if (characterWidget) characterWidget->SetVisibility(ESlateVisibility::Hidden);
		newDragon->GetMesh()->SetVisibility(true);
		newDragon->dragonWidget->SetVisibility(ESlateVisibility::Visible);
		newDragon->dragonWidget->StartPercentageOrbCountDown();
		return;
	}

	
	
}

void ASwordCharacter::PlayerVisible()
{
	GetMesh()->SetVisibility(true);
	overlapSword->SetVisibility(true);
}

void ASwordCharacter::PlayerUnvisible()
{
	GetMesh()->SetVisibility(false);
	overlapSword->SetVisibility(false);
}

void ASwordCharacter::SpawnSequenceDragon()
{
	////////////////////////////////

	if (sequenceDragonClass)
	{
		FVector sDragonSpawnLoc = GetActorLocation() + FVector(0.f, 0.f, 1500.f);
		FVector cameraRightVector = skillCamera->GetRightVector();
		FRotator tempRot = cameraRightVector.Rotation();
		FRotator spawnRot(0.f, tempRot.Yaw + 90.f, 0.f);
		FActorSpawnParameters spawnDragonParams;
		spawnDragonParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FActorSpawnParameters spawnPortalParams;
		spawnPortalParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator portalRot = spawnRot;
		portalRot.Pitch += 90.f;
		sequenceDragonPortal = GetWorld()->SpawnActor<AActor>(sequenceDragonPortalClass, sDragonSpawnLoc, portalRot, spawnDragonParams);
		if (sequenceDragonPortal)
		{
			sequenceDragonPortal->SetActorScale3D(FVector(3.f));
			sequenceDragonPortal->SetLifeSpan(1.5f); 
		}
		sequenceDragon = GetWorld()->SpawnActor<ASequenceDragon>(sequenceDragonClass, sDragonSpawnLoc, spawnRot, spawnDragonParams);

		
		if (sequenceDragon && sequenceDragon->GetMesh())
		{
			sequenceDragon->GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
		}

		if (sequenceDragon)
		{
			dragonStartLocation = sequenceDragon->GetActorLocation();
			dragonTargetLocation = dragonStartLocation - FVector(0.f, 0.f, 1500.f);
			dragonMoveElapsedTime = 0.f;
			//move time
			dragonMoveDuration = 1.5f;

			const float tickInterval = 0.01f;

			GetWorldTimerManager().SetTimer(th_SequenceDragonMove, FTimerDelegate::CreateLambda([this]()
				{
					dragonMoveElapsedTime += 0.01f;
					float moveAlpha = FMath::Clamp(dragonMoveElapsedTime / dragonMoveDuration, 0.f, 1.f);

					float easyTime = FMath::InterpEaseOut(0.f, 1.f, moveAlpha, 2.5f);

					if (sequenceDragon)
					{
						FVector NewLocation = FMath::Lerp(dragonStartLocation, dragonTargetLocation, easyTime);
						sequenceDragon->SetActorLocation(NewLocation);
					}
					if (moveAlpha >= 1.f)
					{
						GetWorldTimerManager().ClearTimer(th_SequenceDragonMove);
						//APlayerController* playerController = Cast<APlayerController>(GetController());
						//if (playerController)
						//{
						//	playerController->ClientStartCameraShake(forceCameraShake);
							
						//}
						
					}
				}), tickInterval, true);
		}
	}
}

void ASwordCharacter::PolyMolfRetrunPlayer()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), returnPlayerParticle, GetActorLocation(), GetActorRotation(), true);
	PlayAnimMontage(montageReturnPlayer);
}

void ASwordCharacter::PossessSetting()
{
	swordActorInstance = Cast<ASwordActor>(overlapSword->GetChildActor());

	playerCurrentForce = 0.0f;
	//Reanual Stat
	myInstance = Cast<UStatGameInstance>(GetGameInstance());
	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));


	//CreateDisplayWidget();
	statGI = Cast<UStatGameInstance>(GetGameInstance());
	swordPlayerController = Cast<ASwordPlayController>(GetController());
	//getDamageDatableValue(14.0f);


	TArray<AActor*> actorsToInclude;

	////minimap
	for (TActorIterator<AActor> actorItr(GetWorld()); actorItr; ++actorItr)
	{
		AActor* actor = *actorItr;
		if (actor != this)
		{
			actorsToInclude.Add(actor);
		}

	}
	minimapCapture->ShowOnlyActors = actorsToInclude;

	//UStatGameInstance 

	UStatGameInstance* GameInstance;
	GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->SwordPlayer = this;
	}

	equipmentUI = CreateWidget<UMainWidget>(swordPlayerController, MainWidgetClass);

	if (equipmentUI)
	{
		equipmentUI->AddToViewport(5);
		equipmentUI->SetVisibility(ESlateVisibility::Visible);

		swordPlayerController->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(equipmentUI->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		swordPlayerController->SetInputMode(InputMode);

		if (UStatGameInstance* GI = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			GI->MainWidgetRef = equipmentUI;
		}
	}
	//blood overlay
	if (bloodWidgetClass)
	{
		bloodWidget = CreateWidget<UBloodWidget>(GetWorld(), bloodWidgetClass);
		if (bloodWidget)
		{
			bloodWidget->AddToViewport();
			bloodWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	
}

void ASwordCharacter::RemoveCharacterWidget()
{
	if (characterWidget && characterWidget->IsInViewport())
	{
		characterWidget->RemoveFromParent();
		characterWidget->ResetTimer();
		characterWidget = nullptr;

	}
}

void ASwordCharacter::ShowAllWidget()
{
	characterWidget->SetVisibility(ESlateVisibility::Visible);
	equipmentUI->SetVisibility(ESlateVisibility::Visible);
	bloodWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASwordCharacter::AfterPolyResetPlayer()
{
	forceAwakeLightning->SetVisibility(false);
	forceBodyLightning->SetVisibility(false);

	if (swordActorInstance && swordActorInstance->swordMesh)
	{
		swordActorInstance->swordMesh->SetMaterial(0, basicSwordMaterial);
	}
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bForceMode = false;
}

void ASwordCharacter::SetLevelMoveUiSave()
{
	if (!characterWidget)
	{
		return;
	}
	if (characterWidget)
	{
		//세이브파일에 있는 값들로 UI초기화
		if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
		{
			UProjectSaveGame* loadedData = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveData"), 0));
			characterWidget->SetSoulCollectionBar(loadedData->saveCurrentSoulGaze);
		}
		
		if (characterWidget->currentSoulGaze == 14.f)
		{
			characterWidget->PlayAnimation(characterWidget->SkillCActivateAnim);
		}
		else
		{
			characterWidget->PlayAnimation(characterWidget->SkillCDeactiveAnim);
		}
	}
}

void ASwordCharacter::SpawnPlayerPortal()
{
	FVector playerLoc = GetActorLocation();
	FRotator playerRot = GetActorRotation();
	FVector portalOffset = -GetActorForwardVector() * 450.f + FVector(0.f, 0.f, 50.f);
	FVector portalSpawnLoc = playerLoc + portalOffset;

	FActorSpawnParameters portalSpawnParams;
	portalSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	polySpawnPlayerPortal = GetWorld()->SpawnActor<AActor>(playerPortal, portalSpawnLoc, playerRot, portalSpawnParams);
}

void ASwordCharacter::DestroyPlayerPortal()
{
	if (polySpawnPlayerPortal)
	{
		polySpawnPlayerPortal->Destroy();
		polySpawnPlayerPortal = nullptr;
	}
}








	
	

