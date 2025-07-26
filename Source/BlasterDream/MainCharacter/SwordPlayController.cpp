// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordPlayController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SwordCharacter.h"
#include "../MainCharacter/CharacterWidget/SwordCharacterWidget.h"
#include "../SpawnActor/SwordActor.h"
#include "Animation/BlendSpace1D.h"
#include "../GameManager/SwordPlayerGameBase.h"
#include "../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MainWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "../MainCharacter/CharacterWidget/RespawnWidget.h"
#include "EngineUtils.h"
#include "../GameManager/ProjectSaveGame.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/BlendSpace1D.h"

UStatGameInstance* GameInstance;

ASwordPlayController::ASwordPlayController()
{
	bShowMouseCursor = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> attackAnimClass(TEXT("/Game/Luco/Animation/ABP_SwordAnimInstanceAction.ABP_SwordAnimInstanceAction_C"));
	if (attackAnimClass.Succeeded())
	{
		attackAnimInstance = attackAnimClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> originAnimClass(TEXT("/Game/Luco/Animation/ABP_SwordAnimInstanceProto.ABP_SwordAnimInstanceProto_C"));
	if (originAnimClass.Succeeded())
	{
		originAnimInstance = originAnimClass.Class;
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;

	//UI
	static ConstructorHelpers::FClassFinder<UMainWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DoveDove/UI/BP_MainWidget.BP_MainWidget_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		MainWidgetClass = WidgetClassFinder.Class;

	}

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void ASwordPlayController::OnPossess(APawn* myPawnPlayer)
{
	Super::OnPossess(myPawnPlayer);

	swordCharacter = Cast<ASwordCharacter>(GetPawn());
	swordCharacter->PossessSetting();
	reloadCurrentTime = 0.0f;

	gameMode = Cast<ASwordPlayerGameBase>(UGameplayStatics::GetGameMode(this));
	GameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	UProjectSaveGame* saveGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));
	mySwordPlayer = Cast<ASwordCharacter>(myPawnPlayer);
	eqipmentWidget = Cast<UMainWidget>(CreateWidget(GetWorld(), MainWidgetClass));
	
	mySwordPlayer->ShowViewPortMainWidget();
	mySwordPlayer->ShowViewPortMainWidgetGameModeVer();
	mySwordPlayer->SetLevelMoveUiSave();
	
	// 세이브 데이터 로드
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
	{
		UProjectSaveGame* loadedData = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveData"), 0));
		if (loadedData && mySwordPlayer)
		{
			mySwordPlayer->playerCurrentHp = loadedData->savePlayerHp;
			mySwordPlayer->playerCurrentMp = loadedData->savePlayerMp;
			GameInstance->playerMoney = loadedData->playerMoney;
			GameInstance->userC = loadedData->userC;

			GameInstance->userGear = loadedData->userGear;
			GameInstance->SlotItems = loadedData->slotItems;
	
			GameInstance->ItemInventory = loadedData->itemInventory;
			mySwordPlayer->equipmentUI->SetInventoryUI();
			eqipmentWidget->changeMoney(loadedData->playerMoney);
			if (mySwordPlayer->characterWidget)
			{
				mySwordPlayer->characterWidget->bGaze14 = loadedData->saveBGaze14;
				mySwordPlayer->characterWidget->currentSoulGaze = loadedData->saveCurrentSoulGaze;

			}
			
		}
	}
	//if no .sav file
	else
	{
		mySwordPlayer->playerCurrentHp = mySwordPlayer->playerMaxHp;
		mySwordPlayer->playerCurrentMp = mySwordPlayer->playerMaxMp;
		eqipmentWidget->changeMoney(1300);
		GameInstance->playerMoney = 1300;
	}

	if (mySwordPlayer->characterWidget->currentSoulGaze == 14.f)
	{
		mySwordPlayer->characterWidget->SetGaze14Gaze();
	}
}

void ASwordPlayController::BeginPlay()
{
	Super::BeginPlay();

	


	
	

	
}

void ASwordPlayController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	//input
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ASwordPlayController::InputMouseRightPressed);
	InputComponent->BindAction("RightClick", IE_Released, this, &ASwordPlayController::InputMouseRightReleased);
	InputComponent->BindAction("Tab", IE_Released, this, &ASwordPlayController::InputTab);
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ASwordPlayController::InputMouseLeftPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ASwordPlayController::InputMouseLeftReleased);
	InputComponent->BindAction("KeyboardQ", IE_Released, this, &ASwordPlayController::InputKeyboardQPressed);
	InputComponent->BindAction("KeyboardW", IE_Released, this, &ASwordPlayController::InputKeyboardWPressed);
	InputComponent->BindAction("KeyboardE", IE_Released, this, &ASwordPlayController::InputKeyboardEPressed);
	InputComponent->BindAction("KeyboardR", IE_Pressed, this, &ASwordPlayController::InputKeyboardRPressed);
	InputComponent->BindAction("KeyboardR", IE_Released, this, &ASwordPlayController::InputKeyboardRReleased);
	InputComponent->BindAction("KeyboardA", IE_Pressed, this, &ASwordPlayController::InputKeyboardAPressed);
	InputComponent->BindAction("KeyboardS", IE_Released, this, &ASwordPlayController::InputKeyboardSPressed);
	InputComponent->BindAction("KeyboardD", IE_Released, this, &ASwordPlayController::InputKeyboardDPressed);
	InputComponent->BindAction("KeyboardF", IE_Released, this, &ASwordPlayController::InputKeyboardFPressed);
	InputComponent->BindAction("KeyboardCtrl", IE_Released, this, &ASwordPlayController::InputKeyboardCtrlPressed);
	InputComponent->BindAction("Keyboard0", IE_Released, this, &ASwordPlayController::InputKeyboard0Pressed);
	InputComponent->BindAction("Keyboard1", IE_Released, this, &ASwordPlayController::InputKeyboard1Pressed);
	InputComponent->BindAction("Keyboard2", IE_Released, this, &ASwordPlayController::InputKeyboard2Pressed);
	InputComponent->BindAction("KeyboardI", IE_Released, this, &ASwordPlayController::InputKeyboardIPressed);
	InputComponent->BindAction("KeyboardP", IE_Released, this, &ASwordPlayController::InputKeyboardPPressed);
	InputComponent->BindAction("KeyboardG", IE_Released, this, &ASwordPlayController::InputKeyboardGPressed);
	InputComponent->BindAction("KeyboardEsc", IE_Released, this, &ASwordPlayController::InputKeyboardEscPressed);
	InputComponent->BindAction("KeyboardC", IE_Released, this, &ASwordPlayController::InputKeyboardCPressed);
}

void ASwordPlayController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// 공격 쿨타임 체크
	if (reloadCurrentTime - lastAttackTime < attackCooldown)
	{
		if (bclickRightMouse && bClickLeftMouseClick && swordCharacter->bHasAttackMode)
		{
			return;
		}
	}
	
	//a skill decal
	if (swordCharacter)
	{
		if (swordCharacter->bAOESpawnTime)
		{
			swordCharacter->spawnSkillIndicatorDecal();

		}
		if (swordCharacter->bAOESpawnTime == false && swordCharacter->bHovering == false)
		{
			swordCharacter->DestroySkillIndicatorDecal();

		}


		if (bClickLeftMouseClick)
		{
			StopMovement();
		}
		if (bclickRightMouse)
		{
			if (!swordCharacter->bHasAttackMode)
			{
				MoveToMouseCursor();
				return;
			}
		}

		//콤보처리
		if (bclickRightMouse && swordCharacter->bHasAttackMode)
		{
			if (swordCharacter->bIsLockKey)
			{
				return;
			}
			if (bClickLeftMouseClick)
			{
				reloadCurrentTime = GetWorld()->GetTimeSeconds();
				if (reloadCurrentTime - lastAttackTime >= attackCooldown + mouseRightClickAndLeftattackDelay)
				{
					if (swordCharacter->comboAttackNum == 0)
					{
						swordCharacter->meleeAttack();
						swordCharacter->comboAttackNum = 1;
					}
					else if (swordCharacter->comboAttackNum == 1)
					{
						swordCharacter->meleeAttack();
						swordCharacter->comboAttackNum = 2;
					}
					else if (swordCharacter->comboAttackNum == 2)
					{
						swordCharacter->meleeAttack();
						swordCharacter->comboAttackNum = 0;
					}

					lastAttackTime = reloadCurrentTime;
				}
			}

			if (!bclickRightMouse)
			{
				swordCharacter->comboAttackNum = 0;
			}

			MoveToMouseCursor();
		}

		//tick
		CheckMouseCursorShape();
	}
	

}

void ASwordPlayController::InputMouseRightPressed()
{
	if (swordCharacter)
	{
		swordCharacter->swordActorInstance->SetDisableAttackCollision();
		if (swordCharacter->bIsLockKey)
		{
			return;
		}
	}
	
	
	bclickRightMouse = true;
}

void ASwordPlayController::InputMouseRightReleased()
{
	if (swordCharacter)
	{
		if (swordCharacter->bIsLockKey)
		{
			return;
		}
	}
	
	bclickRightMouse = false;
}

void ASwordPlayController::InputTab()
{
	if (swordCharacter)
	{
		//changeMode (toggle)
		swordCharacter->bHasAttackMode = !swordCharacter->bHasAttackMode;

		if (swordCharacter->bHasAttackMode)
		{
			USkeletalMeshComponent* mySkeletalMeshComp = swordCharacter->FindComponentByClass<USkeletalMeshComponent>();
			if (mySkeletalMeshComp)
			{
				mySkeletalMeshComp->SetAnimInstanceClass(attackAnimInstance);
				swordCharacter->changeRunMaxWalkSpeed();
			}
			swordCharacter->overlapSword->SetVisibility(true);
			
		}
		else
		{
			USkeletalMeshComponent* mySkeletalMeshComp = swordCharacter->FindComponentByClass<USkeletalMeshComponent>();
			if (mySkeletalMeshComp)
			{
				mySkeletalMeshComp->SetAnimInstanceClass(originAnimInstance);
				swordCharacter->backToOriginMaxWalkSpeed();
			}
			swordCharacter->overlapSword->SetVisibility(false);
		}
	}

}

void ASwordPlayController::InputMouseLeftPressed()
{
	
	if (swordCharacter->bIsLockKey)
	{
		return;
	}
	float currentTime = GetWorld()->GetTimeSeconds();
	if (currentTime - lastClickTime < clickInterval)
	{
		return;
	}
	if (swordCharacter->bHasAttackMode == true)
	{
		swordCharacter->meleeAttack();
	}
	lastClickTime = currentTime;
	bClickLeftMouseClick = true;
}

void ASwordPlayController::InputMouseLeftReleased()
{
	//swordCharacter->swordActorInstance->SetDisableAttackCollision();
	bClickLeftMouseClick = false;

}


void ASwordPlayController::SetNewDestination(const FVector Destination)
{
	
	myPawn = GetPawn();

	if (myPawn)
	{
		float const distance = FVector::Dist(Destination, myPawn->GetActorLocation());
		if (distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
		}
	}
}

void ASwordPlayController::MoveToMouseCursor()
{
	
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (bPlayerDie)
	{
		return;
	}
	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
	}
}

void ASwordPlayController::InputKeyboardQPressed()
{

	if (swordCharacter->bCanUseSkillQ && swordCharacter->bHasAttackMode )//&& swordCharacter->bIsLockKey == false)
	{
		swordCharacter->swordActorInstance->SetDisableAttackCollision();
		swordCharacter->playSkillQ();
		
		if (swordCharacter->characterWidget)
		{
			swordCharacter->characterWidget->updateQCountDown(5.0f);
		}
		if (gameMode->characterWidget)
		{
			gameMode->characterWidget->updateQCountDown(5.0f);
		}
		
	}

}
void ASwordPlayController::InputKeyboardWPressed()
{
	if (swordCharacter->bCanUseSkillW && swordCharacter->bHasAttackMode)//&& swordCharacter->bIsLockKey == false)
	{
		swordCharacter->swordActorInstance->SetDisableAttackCollision();
		swordCharacter->playSkillW();

		if (swordCharacter->characterWidget)
		{
			swordCharacter->characterWidget->updateWCountDown(11.0f);
		}
		if (gameMode->characterWidget)
		{
			gameMode->characterWidget->updateWCountDown(11.0f);
		}
	}
}
void ASwordPlayController::InputKeyboardEPressed()
{
	if (swordCharacter->bCanUseSkillE && swordCharacter->bHasAttackMode)//&& swordCharacter->bIsLockKey == false)
	{
		swordCharacter->swordActorInstance->SetDisableAttackCollision();
		swordCharacter->playSkillE();

		if (swordCharacter->characterWidget)
		{
			swordCharacter->characterWidget->updateECountDown(9.0f);
		}
		if (gameMode->characterWidget)
		{
			gameMode->characterWidget->updateECountDown(9.0f);
		}
	}
}
void ASwordPlayController::InputKeyboardRPressed()
{
	if (swordCharacter->bHasAttackMode)
	{
		if (swordCharacter->bForceMode == false)
		{
			swordCharacter->spawnSkillImpactDecal(0.6);
			swordCharacter->aoeRadius = 558.0f;
			swordCharacter->spawnAOEDecal(2.2f);
		}
		if (swordCharacter->bForceMode)
		{
			swordCharacter->spawnSkillImpactDecal(0.6);
			swordCharacter->aoeRadius = 958.0f;
			swordCharacter->spawnAOEDecal(3.8f);
		}
		
		swordCharacter->swordActorInstance->SetDisableAttackCollision();
		bClickRHolding = true;
		
	}
	
}
void ASwordPlayController::InputKeyboardRReleased()
{


	if (swordCharacter->bHasAttackMode)
	{
		bClickRHolding = false;;
		if (swordCharacter->bCanUseSkillR && swordCharacter->bRTooHigh == false && swordCharacter->bInCircle)
		{
			swordCharacter->playSkillR();

			if (swordCharacter->characterWidget)
			{
				swordCharacter->characterWidget->updateRCountDown(7.0f);
				
			}
			if (gameMode->characterWidget)
			{
				gameMode->characterWidget->updateRCountDown(7.0f);
				
			}
		}

		swordCharacter->spawnDecal->setMPCScalar0Value();
		swordCharacter->DestroyImpactIndicatorDecal();
		swordCharacter->DestroyAOEDecal();
	}
	
	
	
}
void ASwordPlayController::InputKeyboardAPressed()
{
	
	if (swordCharacter->bHasAttackMode &&swordCharacter->bForceMode &&swordCharacter->bCanUseSkillA)
	{
		
		swordCharacter->playSkillA();
		
		

	}
	if (swordCharacter->bForceMode == false && swordCharacter->bHovering)
	{
		swordCharacter->OnSkillEndSkillA(nullptr, false);
	}
	
	
}

void ASwordPlayController::InputKeyboardSPressed()
{
	if (swordCharacter->bHasAttackMode)
	{
		swordCharacter->playSkillS();
	}
}
void ASwordPlayController::InputKeyboardDPressed()
{
}
void ASwordPlayController::InputKeyboardFPressed()
{
	GameInstance->PrintInven();
}

void ASwordPlayController::InputKeyboardCtrlPressed()
{
	if (swordCharacter->bHasAttackMode)
	{
		swordCharacter->playForceAwake();
	}
	
}

//cheat
void ASwordPlayController::InputKeyboard0Pressed()
{
	GameInstance->PrintInven();

	//cheat
	//swordCharacter->playSkillF();
}

void ASwordPlayController::InputKeyboard1Pressed()
{
	GameInstance->usingItem(TEXT("HP_potion"));
}

void ASwordPlayController::InputKeyboard2Pressed()
{
	GameInstance->usingItem(TEXT("MP_potion"));
}

void ASwordPlayController::InputKeyboardIPressed()
{
	GameInstance->visbleUI(TEXT("Inven"));
}

void ASwordPlayController::InputKeyboardPPressed()
{
	GameInstance->visbleUI(TEXT("UserInstal"));
}

void ASwordPlayController::InputKeyboardGPressed()
{
	if (swordCharacter->currentInteractionNpc)
	{
		swordCharacter->currentInteractionNpc->StartInteractionCamera();
	}
}

void ASwordPlayController::InputKeyboardEscPressed()
{
	swordCharacter->SetPauseWidget();
}

void ASwordPlayController::InputKeyboardCPressed()
{
	if (mySwordPlayer->characterWidget->currentSoulGaze == 14.f)
	{
		swordCharacter->SpawnPlayerPortal();
		swordCharacter->Polymorph();
		bCanPoly = false;
	}
}




void ASwordPlayController::getMousePosition()
{
	GetHitResultUnderCursor(ECC_Visibility, false, skillMouseCursorHit);
}
void ASwordPlayController::getCurrentActorLocAndRot()
{
	if (swordCharacter)
	{
		currentPlayerLoc = swordCharacter->GetActorLocation();
		currentPlayerRot = swordCharacter->GetActorRotation();
	}

	
}

void ASwordPlayController::CheckMouseCursorShape()
{
	FHitResult mouseHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, mouseHitResult);
	if (mouseHitResult.bBlockingHit)
	{
		AActor* hitActor = mouseHitResult.GetActor();
		if (hitActor && hitActor->GetName().Contains(TEXT("Crow"))|| hitActor && hitActor->GetName().Contains(TEXT("Skeleton")))
		{
			if (!bOnMouseCursorRecord)
			{
				bOnMouseCursorRecord = true;
				CurrentMouseCursor = EMouseCursor::Crosshairs;
			}
			return;
		}

		if (bOnMouseCursorRecord)
		{
			bOnMouseCursorRecord = false;
			CurrentMouseCursor = EMouseCursor::Default;
		}
	}
}

void ASwordPlayController::BlockingSkillKey()
{
	// Tab,
	InputComponent->BindAction("Tab", IE_Pressed, this, &ASwordPlayController::BlockKey);
}

void ASwordPlayController::BlockKey()
{
	
}

void ASwordPlayController::RevivePlayer(ASwordCharacter* dyingCharacter)
{
	bPlayerDie = true;

	// 위젯 타이머 및 제거 처리
	if (dyingCharacter && dyingCharacter->characterWidget)
	{
		dyingCharacter->characterWidget->GetWorld()->GetTimerManager().ClearTimer(dyingCharacter->characterWidget->th_InnerProgressTimer);
		dyingCharacter->characterWidget->RemoveFromParent();
		dyingCharacter->characterWidget = nullptr;

		if (gameMode->characterWidget)
		{
			gameMode->characterWidget->GetWorld()->GetTimerManager().ClearTimer(gameMode->characterWidget->th_InnerProgressTimer);
			gameMode->characterWidget->RemoveFromParent();
			gameMode->characterWidget = nullptr;
		}
	}

	if (dyingCharacter)
	{
		playerSpawnLoc = dyingCharacter->GetActorLocation();
		playerSpawnRot = dyingCharacter->GetActorRotation();
	}

	grayColorGradingSet();

	// 죽음 애니메이션 재생
	dyingCharacter->TriggerDeathAndRevive(true);
	DisableInput(this);

	if (swordCharacter)
	{
		if (UCapsuleComponent* capsuleComp = swordCharacter->FindComponentByClass<UCapsuleComponent>())
		{
			capsuleComp->SetGenerateOverlapEvents(false);
		}
	}

	// 임시 카메라 생성 및 전환
	FVector cameraLoc = PlayerCameraManager->GetCameraLocation();
	FRotator cameraRot = PlayerCameraManager->GetCameraRotation();
	FActorSpawnParameters spawnTempCameraParams;
	ACameraActor* cameraHolder = GetWorld()->SpawnActor<ACameraActor>(cameraLoc, cameraRot, spawnTempCameraParams);
	if (cameraHolder && cameraHolder->GetCameraComponent())
	{
		cameraHolder->GetCameraComponent()->SetActive(true);
	}

	SetViewTargetWithBlend(cameraHolder, 0.1f);

	// 리스폰 UI 띄우기
	if (respawnWidget)
	{
		URespawnWidget* respawnWidgetInstance = CreateWidget<URespawnWidget>(this, respawnWidget);
		if (respawnWidgetInstance)
		{
			respawnWidgetInstance->AddToViewport(11);
		}
	}
}

void ASwordPlayController::SpawnCharacter()
{	
	bPlayerDie = false;
	grayColorGradingUnSet();

	UStatGameInstance* statGameInstance = Cast<UStatGameInstance>(GetWorld()->GetGameInstance());
	if (statGameInstance)
	{
		statGameInstance->ResetHpMp();
		statGameInstance->setDungeonReviveCounting();
	}

	// 캐릭터가 이미 있으면 새로 스폰하지 않고 재사용
	if (!swordCharacter)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		swordCharacter = GetWorld()->SpawnActor<ASwordCharacter>(respawnCharacterClass, playerSpawnLoc, playerSpawnRot, spawnParams);
	}
	else
	{
		// 이미 존재하는 캐릭터 위치, 회전 초기화
		swordCharacter->SetActorLocation(playerSpawnLoc);
		swordCharacter->SetActorRotation(playerSpawnRot);
	}

	// 부활 애니메이션 등 상태 초기화
	swordCharacter->TriggerDeathAndRevive(false);

	// 저장된 세이브 데이터 있으면 적용
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
	{
		UProjectSaveGame* loadedData = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveData"), 0));
		if (loadedData)
		{
			loadedData->savePlayerHp = swordCharacter->playerMaxHp;
			loadedData->savePlayerMp = swordCharacter->playerMaxMp;
		}
	}

	// 리스폰 이펙트(나이아가라) 재생
	if (respawnNa)
	{
		FVector niagarSpawnLoc = swordCharacter->GetActorLocation();
		niagarSpawnLoc.Z -= 90.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), respawnNa, niagarSpawnLoc);
	}

	// 컨트롤러가 캐릭터를 다시 Possess
	Possess(swordCharacter);
	swordCharacter->swordPlayerController = this;

	DisableInput(this);
	InputTab();

	FTimerHandle th_RespawnMontage;
	GetWorld()->GetTimerManager().SetTimer(th_RespawnMontage, [this]()
		{
			if (swordCharacter && swordCharacter->respawnMontage)
			{
				UAnimInstance* animInst = swordCharacter->GetMesh()->GetAnimInstance();
				if (animInst)
				{
					animInst->Montage_Play(swordCharacter->respawnMontage);
				}
			}
		}, 0.1f, false);

	float spawnAnimLength = swordCharacter->respawnMontage ? swordCharacter->respawnMontage->GetPlayLength() : 0.f;

	FTimerHandle th_reviveAfter;
	GetWorld()->GetTimerManager().SetTimer(th_reviveAfter, [this]()
		{
			if (swordCharacter)
			{
				if (UCapsuleComponent* capsuleComp = swordCharacter->FindComponentByClass<UCapsuleComponent>())
				{
					capsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					capsuleComp->SetCollisionProfileName(TEXT("Pawn"));
					capsuleComp->SetGenerateOverlapEvents(true);
				}
			}
			EnableInput(this);
		}, spawnAnimLength, false);
}

void ASwordPlayController::grayColorGradingSet()
{
	for (TActorIterator<APostProcessVolume> it(GetWorld()); it; ++it)
	{
		APostProcessVolume* pv = *it;
		if (pv)
		{
			pv->bUnbound = true; 
			pv->Settings.bOverride_ColorSaturation = true;  
			pv->Settings.ColorSaturation = FVector(0.3f, 0.3f, 0.3f); 
		}
	}
}

void ASwordPlayController::grayColorGradingUnSet()
{
	for (TActorIterator<APostProcessVolume> it(GetWorld()); it; ++it)
	{
		APostProcessVolume* pv = *it;
		if (pv)
		{
			pv->bUnbound = true; 
			pv->Settings.bOverride_ColorSaturation = true;  
			pv->Settings.ColorSaturation = FVector(1.f, 1.f, 1.f); 
		}
	}
}

void ASwordPlayController::InteractionHidePlayerWidget()
{
	swordCharacter->characterWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ASwordPlayController::InteractionShowPlayerWidget()
{
	swordCharacter->characterWidget->SetVisibility(ESlateVisibility::Visible);
}







