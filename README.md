# Project Seed - Unreal Portfolio

포트폴리오 목표로 제작된 탑다운 RPG 데모 게임입니다.<br/>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 엔진 : Unreal Engine 5.4 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 규모 : 2인 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 제작기간 : 50일 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 담당부분 : 인벤토리 제외 전부 <br/>

# 사용한 개발 도구<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● Microsoft Visual Studio <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blender<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● GithubDesktop<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Github, GithubLFS<br/>

# 데모 영상 
| 시연영상 
| <a href="https://youtu.be/GQQTKInkYU4?si=LuZnMZ9ZGrvhFbc3"><img width="1920" height="1080" alt="Image" src="https://github.com/user-attachments/assets/0c4450f8-6378-4a69-a7a2-1d770803d3a5" /> </a> 
| (이미지 클릭시 유튜브로 이동합니다) | <br/>



# Packaging File
> **DownLoad Link**<br/>
● https://drive.google.com/file/d/1BxTrYxhr6COFHqMP94oGQUsoWQQSigWw/view?usp=sharing <br/>


# 기술 설명
<br/>

## Player
직접 조작 가능한 메인 캐릭터<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● 탑다운 시점의 마우스이동, 키보드조작 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine, BlendSpace를 활용한 자연스러운 움직임 전환 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 플레이어와 연동된 여러가지 기능의 위젯 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 쉽게 추가할수있는 콤보, 스킬 몽타쥬 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● PostProcess를 사용한 여러가지 효과<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Decal을 사용한 인디케이터<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● MotionWarping을 사용한 자연스러운 플레이어 움직임<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 직교투영 미니맵<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 넉백, 렉돌, 데미지Hud를 사용한 전투경험<br/>

> **Player 설계도**<br/>

<div align="center">
  <h3 style="text-align: center; font-weight: bold;">Action Mapping을 사용한 액션바인드</h3>
  <img src="https://github.com/user-attachments/assets/aaf2363e-8e46-477e-841e-d0bfd83de486" alt="Action Mapping 이미지">
</div>

<table style="width: 100%;">
  <tr>
    <th style="width: 50%;">플레이어 메인 위젯</th>
    <th style="width: 50%;">던전 위젯</th>
  </tr>
  <tr>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/30775e6b-355d-4e4c-b8d2-57022ec214bf" style="width: 100%; height: auto;" />
    </td>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/d41d398e-7bb1-4336-bf00-35e4833c05a1" style="width: 100%; height: auto;" />
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center" width="50%">
      <strong>메인화면 위젯 애니메이션</strong><br>
      <img src="https://github.com/user-attachments/assets/3ff1b959-8a14-404f-a39c-cf0e0fa5892e" width="100%" />
    </td>
    <td align="center" width="50%">
      <strong>던전 위젯 애니메이션</strong><br>
      <img src="https://github.com/user-attachments/assets/34fc52c1-f234-4a2d-b11e-bf1bb35379fb" width="100%" />
    </td>
  </tr>
  <tr>
    <td colspan="2" align="center" style="border: 2px solid #ccc; padding: 10px;">
      <strong>캐릭터 위젯 애니메이션</strong><br>
      <img src="https://github.com/user-attachments/assets/c345f613-e967-4573-a4e4-ae55a52d014f" width="100%" />
    </td>
  </tr>
</table>


<div align="center">
  <h3 style="text-align: center; font-weight: bold;">MotionWarping을 사용한 부드러운 방향전환</h3>
  <img src="https://github.com/user-attachments/assets/dce34d96-b3dd-43fd-b387-5b02aa9c8d4f" alt="모션워핑">	
</div>

<details>
<summary><strong>📌 마우스 위치 기반 MotionWarping 코드</strong></summary>

```cpp
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

        bool bHit = GetWorld()->SweepSingleByChannel(
            moveHit,
            GetActorLocation(),
            desiredLocation,
            FQuat::Identity,
            ECC_Visibility,
            FCollisionShape::MakeSphere(sphereRadius),
            params
        );

        FVector finalLocation = bHit ? GetActorLocation() : desiredLocation;

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
    }
}
```
</details>
<br/>


# Player 스킬 표현

데칼(Decal)과 PostProcess를 활용하여 스킬 및 Blur 효과를 구현하였습니다.

## 🎯 데칼 (Decal)

아래 이미지는 데칼을 사용한 스킬 표현 예시입니다.

<table width="100%">
  <tr>
    <td width="50%">
      <img src="https://github.com/user-attachments/assets/a684f319-dd3f-4eff-9740-d78595073f6f" width="100%" alt="Decal Example 1" />
    </td>
    <td width="50%">
      <img src="https://github.com/user-attachments/assets/042b867e-8617-4d1c-be2f-389b52974f9c" width="100%" alt="Decal Example 2" />
    </td>
  </tr>
</table>

[데칼 제작과정 자세히 보기](https://lucodev.tistory.com/13)

---

## 🎯 신속 Blur 효과

아래 이미지는 PostProcess를 이용해 구현한 신속 Blur 효과입니다.

<table style="width: 100%;">
  <tr>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/4e5fe6d7-1b85-4585-8f61-ab533f6f45e4" style="width: 100%; height: auto;" alt="Quick Blur Effect" />
    </td>
  </tr>
</table>

[신속 Blur 제작과정 자세히 보기](https://lucodev.tistory.com/32)

---


> **데이터 관리**<br/>

DataTable을 사용한 데이터입력 및 수정<br/>
<table style="width: 100%;">
  <tr>
    <th style="width: 50%;">스킬 수치 및 계수 Data</th>
    <th style="width: 50%;">상점 아이템 배열 및 정보</th>
  </tr>
  <tr>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/09a7679f-f982-4fde-b7ba-ba5c42ab8fbf" style="width: 100%; height: auto;" />
    </td>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/9cf4d39c-9301-4a1d-a108-d19326a96e5c" style="width: 100%; height: auto;" />
    </td>
  </tr>
</table>
<br/>

SaveGame API를 사용한 데이터 저장 및 불러오기  
<table>
  <tr>
    <td align="center" style="border: 1px solid #888; padding: 4px;">
      <img src="https://github.com/user-attachments/assets/699a1698-f02b-4da4-ab73-35a33c311733" alt="SaveGame 성공 예시" width="300" />
    </td>
    <td align="center" style="border: 1px solid #888; padding: 4px;">
      <img src="https://github.com/user-attachments/assets/5b14f736-8847-4daf-a088-3d899960dbbb" alt="SaveGame" width="300" />
    </td>
  </tr>
</table>
<details>
<summary><strong>📌 게임 세이브 코드</strong></summary>

```cpp
//save data File Name
saveGameInstance->saveSlotName = "PlayerSaveData";
saveGameInstance->saveIndex = 0;

saveGameInstance->savePlayerHp = playerSaveHp;
saveGameInstance->savePlayerMp = playerSaveMp;

//loc, rot, map
saveGameInstance->playerLastLocation = playerCharacter->GetActorLocation();
saveGameInstance->playerLastRotation = playerCharacter->GetActorRotation();
FString mapName = GetWorld()->GetMapName();

//Crits Percent, Money
saveGameInstance->userC = userC;
saveGameInstance->playerMoney = playerMoney;

//inven custom function
InvenSlotSave();
userGearSlotSave();
	
saveGameInstance->userGear = userGear;
saveGameInstance->slotItems = SlotItems;
saveGameInstance->itemInventory = ItemInventory;

mapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
saveGameInstance->lastMapName = mapName;

//polymolf gaze
saveGameInstance->saveBCanPoly = instanceBCanPoly;
saveGameInstance->saveBGaze14 = instanceBGaze14;
saveGameInstance->saveCurrentSoulGaze = instanceCurrentSoulGaze;
UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->saveIndex);
```
</details>
<br/>

게임 모드를 별 레벨의 다양한 게임 규칙이 적용.

<table align="center" style="border-collapse: separate; border-spacing: 0 20px; max-width: 320px; margin: auto;">
  <tr>
    <td align="center" style="border: 1px solid #888; padding: 4px; width: 320px; height: 180px;">
      <img src="https://github.com/user-attachments/assets/883ad0be-770b-4a62-b165-95f683521507" alt="SaveGame 성공 예시" style="width: 100%; height: 100%; object-fit: cover;" />
    </td>
  </tr>
  <tr>
    <td align="center" style="border: 1px solid #888; padding: 4px; width: 320px; height: 180px;">
      <img src="https://github.com/user-attachments/assets/633b6321-d380-4ded-b8da-ee7b787f1c14" alt="SaveGame" style="width: 100%; height: 100%; object-fit: cover;" />
    </td>
  </tr>
</table>

## Enemy AI
던전에서 스폰되는 AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 8방향 BlendSpace의 자연스러운 움직임 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Interface를 사용한 플레이어와의 상호작용 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● AI-Perception <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● RVO, EQS 알고리즘 사용 <br/>

기본 BehaviorTree 신호 및 설계도

<img src="https://github.com/user-attachments/assets/2fb53dcc-b788-439d-b1d9-215db17307a4" alt="Behavior Tree 동작 방식" width="100%" />
<br><br>

> **Behavior Tree** <br/>
AI의 동작방식 설계도입니다
>
> 
&nbsp;&nbsp;&nbsp;&nbsp;● 구동방식 : Service노드에서의 EnumClass로 정해진 State 변형 <br>

| 기본 트리 구성 | 추가 트리 구성 |
|:--------------:|:-------------------:|
| <img src="https://github.com/user-attachments/assets/5566b686-4456-4c66-922a-51697a04f943" alt="BehaviorTree Left" width="100%"/> | <img src="https://github.com/user-attachments/assets/d854d143-313e-4efa-ac8c-8327227b4cc5" alt="BehaviorTree Right" width="100%"/> |

## 🎯 EQS 시스템 (Environment Query System)

> **EQS 모델 설명**  
> Strafe 이동을 위한 EQS 시스템<br>
> 환경 쿼리, 점수화, 필터링 과정을 통해 적절한 위치를 선택합니다

<div align="center">
  
![EQS 시스템 구성도](https://github.com/user-attachments/assets/10873efa-9d43-4cd3-96b8-c866aa892b92)

![eqs움짤 이미지](https://github.com/user-attachments/assets/1977e5a8-a9a9-4b4b-84fc-18b9d6992eea)

</div>

> **8방향 BlendSpace**  
> 부드러운 움직임을 위한 모션세팅<br>

![8BlendSpace움짤](https://github.com/user-attachments/assets/7aa9b22c-615c-4586-9243-7dd4a0a03b3f)

<details>
<summary><strong>📌 BlendSpace 설계도 </strong></summary>

<br/>

![8BlendSpace설계도사진](https://github.com/user-attachments/assets/84b2df3a-e4f4-4df1-bbd2-5308c6a92f8f)

</details>

> **스폰과 디스폰을 위한 DissolveMaterial**  
> 스칼라 파라미터의 제어로 스폰과 디스폰의 연출<br>
> 머티리얼에 설정된 DissolveMask와 노이즈 텍스처를 활용해, 캐릭터가 점차 나타나거나 사라지는 시각 효과를 구현합니다.<br>
> **C++ 코드에서 Material Parameter Collection(MPC)**의 Scalar 값을 직접 조절하여 Dissolve 효과를 제어하며,<br>
> 이를 통해 연출 타이밍을 정밀하게 컨트롤할 수 있고, 여러 오브젝트에 일괄 적용하는 것도 가능합니다.

> 🔗 [내 티스토리 블로그에서 제작 과정 보기](https://lucodev.tistory.com/48)

![스폰사진](https://github.com/user-attachments/assets/6ca11969-25d8-46c8-974b-2d4480e5f318)

<details>
<summary><strong>📌 DissolveMaterial 제어 Task 코드 </strong></summary>

```cpp
void UTask_SkeletonSpawn::TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds)
{
	Super::TickTask(ownerComp, nodeMemory, deltaSeconds);
	FTaskSkeletonSpawnMemory* taskMemory = reinterpret_cast<FTaskSkeletonSpawnMemory*>(nodeMemory);

	taskMemory->structCurrentTime += deltaSeconds;
	UBlackboardComponent* blackBoard = ownerComp.GetBlackboardComponent();
	if (taskMemory->structCurrentTime < 3.0f)
	{
		
		float dissolveValue = FMath::Lerp(maxDissolveValue, minDissolveValue, taskMemory->structCurrentTime / 3.0f);
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		for (int32 i = 0; i < 3; ++i)
		{
			int32 materialIndex = i;

			UMaterialInstanceDynamic* dynMat = skeleton->GetMesh()->CreateAndSetMaterialInstanceDynamic(materialIndex);
			if (dynMat)
			{
				dynMat->SetScalarParameterValue("Dissolve", dissolveValue);
			}
		}
		if (skeleton->sword)
		{
			int32 swordMatCount = skeleton->sword->GetNumMaterials();
			for (int32 i = 0; i < swordMatCount; ++i)
			{
				UMaterialInstanceDynamic* swordMat = skeleton->sword->CreateAndSetMaterialInstanceDynamic(i);
				if (swordMat)
				{
					swordMat->SetScalarParameterValue("Dissolve", dissolveValue);
				}
			}
		}
	}
	else
	{
		ABasicSkeletonEnemy* skeleton = Cast<ABasicSkeletonEnemy>(ownerComp.GetAIOwner()->GetPawn());
		blackBoard->SetValueAsBool("Spawn", true);
		FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	}
}
```

</details>

## EnumClass를 사용한 BlackBoardKey제어  
BehaviorTree의 Task에서 Enum 값을 기반으로 AI의 상태를 제어합니다. <br/>
서비스 노드에서 설정된 Enum 값을 읽어와, 해당 상태에 따라 Task를 분기 실행합니다.<br/>

<img src="https://github.com/user-attachments/assets/6220ac42-0326-4446-b9dd-34eaf99d6733" alt="enumclass사진1" width="600" />

<img src="https://github.com/user-attachments/assets/f3b4a923-b598-4b63-8e1e-925217316862" alt="enumclass사진2" width="600" />

> 🔗 [내 티스토리 블로그에서 제작 과정 보기](https://lucodev.tistory.com/42)


## Dragon Polymorph
드래곤으로 변신 가능한 캐릭터<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine으로 제어하는 여러가지 BlendSpace <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 컨트롤Rig을 변형한 애니메이션 시퀀스 베이크 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Layered Blend Per Bone 노드를 활용해 목 위와 목 아래를 개별적으로 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Projectile 타입 투사체 발사가 가능 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 변신 시 고유 시네마틱 컷씬이 재생됨 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Poccess방식 컨트롤러 제어 <br/>

> **고유 시네 컷씬**  
> 플레이어의 게이지가 Max일 경우 변신 가능<br>

![Image](https://github.com/user-attachments/assets/f72e38e3-325b-446c-b90a-e855c28e8f2c) </div>

> **TakeOff / Landing** <br/>
> 캐릭터의 이동 상태를 **Flying** 모드로 전환하여,<br/>
물리 기반 중력 효과를 비활성화하고 공중에 부유하는 동작을 구현하였습니다.<br/>
이를 통해 공중 제어 및 애니메이션 전환이 매끄럽게 이루어지도록 설계하였습니다.
<table style="width: 100%;">
  <tr>
    <th style="width: 50%;">Take Off</th>
    <th style="width: 50%;">Landing</th>
  </tr>
  <tr>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/89298f03-1ecb-4746-ba2e-d4d1531d5033" style="height: 300px;" />
    </td>
    <td style="width: 50%;">
      <img src="https://github.com/user-attachments/assets/1964d324-54aa-497e-b606-5c1540b0d741" style="height: 300px;" />
    </td>
  </tr>
</table>

> **State Machine을 기반으로 한 움직임변경**  
> AnimInstance를 활용한 State Machine 기반의 BlendSpace 제어<br>

![Image](https://github.com/user-attachments/assets/0f26084b-d34f-414f-8d14-b7d3d50a6b36) </div>

## 🎞️ State Machine 기반 BlendSpace 제작 과정

---

### 1. 🎛 Control Rig 기반 애니메이션 제작  
Control Rig를 사용해 직접 포즈 및 움직임을 제어하며 애니메이션 시퀀스를 제작합니다.

<img src="https://github.com/user-attachments/assets/f8f6207e-b166-4b44-910c-e448c1869379" alt="ControlRig 제작" width="600"/>

---

### 2. 🔥 애니메이션 시퀀스 베이킹 후 BlendSpace에 연동  
Control Rig로 제작한 시퀀스를 Bake하여 애니메이션 자산으로 변환한 뒤, 이를 BlendSpace에 연결하여 다양한 방향 전환/속도에 따른 자연스러운 전환을 구성합니다.

<img src="https://github.com/user-attachments/assets/e0d24d5d-496c-4588-8549-400f6e115302" alt="BlendSpace 연동" width="600"/>

---

### 3. 🦴 SkeletalMesh의 Neck 본 별도 제어  
상체의 시선 방향이나 고개 움직임을 세밀하게 제어하기 위해, Neck 본의 회전은 별도로 AnimInstance에서 제어합니다.

<img src="https://github.com/user-attachments/assets/5a1f46fa-e4f3-4074-aa0a-6429357e685c" alt="Neck 회전 적용 1" width="600"/>
<br/>
<img src="https://github.com/user-attachments/assets/f14c24f1-7358-4ed6-9d42-b1f77f2d59a0" alt="Neck 회전 적용 2" width="600"/>

### 4. 🎮 Possess 함수로 캐릭터 컨트롤러 연결  
`PlayerController` 가 캐릭터를 소유(Possess)하도록 설정합니다.  
이 과정으로 캐릭터와 드래곤의 컨트롤러 주고받을수있습니다
<details>
<summary><strong>📌 Possess (폴리모프) 코드 </strong></summary>

```cpp
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
```

</details>


## 📌 결과

AnimInstance에서 StateMachine과 BlendSpace를 함께 활용하여,  
**다양한 방향성과 속도에 따라 자연스럽게 전환되는 애니메이션 시스템**을 구성하였습니다.  
또한, **Neck 본을 개별 제어**함으로써 상체의 움직임과 시선 처리에 더욱 디테일한 연출이 가능하도록 하였습니다.  

<br/>

<p align="center">
  <img src="https://github.com/user-attachments/assets/f1f32033-d59f-4357-83f2-b84f8da62d53" alt="BlendSpace 결과 이미지" width="600"/>
  <br/>
  <em>▲ 최종 BlendSpace 연동 결과</em>
</p>

## Interaction
플레이어와 상호작용되는 오브젝트 및 시스템<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 피직스 메테리얼 FootStep 및 사운드 관리<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● UI Absorption Effect<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 마스터 필드를 사용한 Fracture 연출<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Npc<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 비동기 로딩<br/>

> **피직스 메테리얼 FootStep 및 사운드 관리**  
> LandScape 레벨 레이어 발소리 및 이펙트 출력<br>

## 피직스 메테리얼 FootStep 및 사운드 관리  
LandScape 레벨 레이어 발소리 및 이펙트 출력. <br/>
AudioComponent 및 사운드큐를 사용한 사운드 관리. <br/>

<img src="https://github.com/user-attachments/assets/5579af27-e638-4460-ad08-f0957ab7d3ec" alt="footstepimage" width="600" />

<details>
<summary><strong>📌 LandScape Physical Material FootStep 코드 </strong></summary>

```cpp
//landscape
FHitResult randScapeHitResult = {};
FCollisionQueryParams randScapeParam = {};
randScapeParam.bReturnPhysicalMaterial = true;
randScapeParam.AddIgnoredActor(this);
bool bHitRandScape = GetWorld()->LineTraceSingleByChannel(randScapeHitResult, startPoint, endPoint, ECC_GameTraceChannel6, randScapeParam);

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
			//Grass
		case SurfaceType5:
			if (sound_Grass.Num() > 0)
			{
				int32 grassIndex = FMath::RandRange(0, sound_Grass.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Grass[grassIndex], hit.ImpactPoint);
			}
			break;
			//Water
		case SurfaceType6:
			if (sound_Water.Num() > 0)
			{
				int32 waterIndex = FMath::RandRange(0, sound_Water.Num() - 1);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound_Water[waterIndex], hit.ImpactPoint);
			}
			break;
		case SurfaceType7:
			//Stone
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
}
```

</details>



## AudioComponent를 사용한 BGM 관리<br/>

<img src="https://github.com/user-attachments/assets/4ddaf1ac-7531-495a-bfb5-d8753d667a22" />

<details>
<summary><strong>📌 Audio Component PlayBGM 커스텀함수 </strong></summary>

```cpp
void ASwordPlayerGameBase::PlayBGM(USoundBase* bgm)
{
	if (!IsValid(currentBGM) || !currentBGM->IsRegistered())
	{
		currentBGM = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass(), TEXT("BGM_AudioComponent"));
		if (!currentBGM) return;

		currentBGM->bAutoActivate = false;
		currentBGM->bIsUISound = false;
		currentBGM->bAllowSpatialization = false;
		currentBGM->bOverridePriority = true;
		currentBGM->Priority = 100.0f;

		currentBGM->RegisterComponentWithWorld(GetWorld());
		currentBGM->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (currentBGM->IsPlaying())
	{
		currentBGM->Stop();
	}

	currentBGM->SetSound(bgm);
	float fadeBGMInDuration = 2.0f;
	float bgmVolume = 1.0f;
	float bgmStartTime = 0.0f;
	currentBGM->FadeIn(fadeBGMInDuration, bgmVolume, bgmStartTime);
}
```

</details>

## UI Absorption Effect 위젯
3D -> 2D UI의 좌표변환이동  <br/>
좌표로 변환한 뒤, 해당 위치로 액터를 보간 이동  <br/>

<img src="https://github.com/user-attachments/assets/06f6210d-1e0e-4309-a5be-6b6d89580050" />

<details>
<summary><strong>📌 Audio Component PlayBGM 커스텀함수 </strong></summary>

```cpp
void ASoulActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!playerCon) return;

    ASwordCharacter* swordChar = Cast<ASwordCharacter>(playerCon->GetPawn());
    if (!swordChar || !swordChar->characterWidget) return;

    FVector2D holeScreenPos = swordChar->characterWidget->GetSoulHoleScreentPosition();
    targetScreenPosition = holeScreenPos;

    FVector worldOrigin, worldDirection;
    if (playerCon->DeprojectScreenPositionToWorld(targetScreenPosition.X, targetScreenPosition.Y, worldOrigin, worldDirection))
    {
        float distance = 1000.f;
        FVector targetWorldLocation = worldOrigin + worldDirection * distance;

        FVector currentLocation = GetActorLocation();
        FVector direction = (targetWorldLocation - currentLocation).GetSafeNormal();

        float speed = 200.f;
        FVector newLocation = currentLocation + direction * speed * DeltaTime;

        if (FVector::Dist(newLocation, targetWorldLocation) < 5.f)
        {
            newLocation = targetWorldLocation;
        }

        SetActorLocation(newLocation);
    }
}
```

</details>

## MaterField / AnchorField 를 사용한 Fracture 연출
벽이 부서지는 시네 컷씬 연출

<img src="https://github.com/user-attachments/assets/70e09cd4-f9ba-4dd0-9c16-9de8f698ca8b" />
<img src="https://github.com/user-attachments/assets/54e6b1f4-cd83-4e9d-b810-6486309694c9" />

✅ 멀티플레이 환경구축<br/>
✅ BehaviorTree를 사용한 AI 제작<br/>
✅ 크래프팅 시스템의 구현<br/>
✅ 한사이클이 돌아가는 rpg 게임 구현<br/>



