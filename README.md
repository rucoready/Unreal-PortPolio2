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

<div align="center">
  <h3 style="text-align: center; font-weight: bold;">MotionWarping을 사용한 부드러운 방향전환</h3>
  <img src="https://github.com/user-attachments/assets/aad21514-07c2-4719-9624-bd5ea4122dda" alt="모션워핑">	
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

> **데이터 관리**<br/>

DataTable을 사용한 데이터입력 및 수정<br/>
<table style="width: 100%;">
  <tr>
    <th style="width: 50%;">플레이어 스킬 계수 및 데미지</th>
    <th style="width: 50%;">상점 아이템</th>
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

> **Behavior Tree**
아래는 Behavior Tree의 구조와 실행 흐름을 보여주는 다이어그램입니다.
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
## EnumClass를 사용한 BlackBoardKey제어
(설명)<br/>

![enumclass사진1](https://github.com/user-attachments/assets/6220ac42-0326-4446-b9dd-34eaf99d6733)

![enumclass사진2](https://github.com/user-attachments/assets/f3b4a923-b598-4b63-8e1e-925217316862)
> 🔗 [내 티스토리 블로그에서 제작 과정 보기](https://lucodev.tistory.com/42)

</details>

## Dragon Polymorph
드래곤으로 변신 가능한 캐릭터<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine으로 제어하는 여러가지 BlendSpace <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 컨트롤Rig을 변형한 애니메이션 시퀀스 베이크 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Layered Blend Per Bone 노드를 활용해 목 위와 목 아래를 개별적으로 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Projectile 타입 투사체 발사가 가능 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 변신 시 고유 시네마틱 컷씬이 재생됨 <br/>

> **고유 시네 컷씬**  
> 플레이어의 게이지가 Max일 경우 변신 가능<br>

![Image](https://github.com/user-attachments/assets/f72e38e3-325b-446c-b90a-e855c28e8f2c) </div>



## 오브젝트
플레이어를 도와주거나 방해하거나 디테일이 추가된 요소<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 경험치 구슬<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 확률성 아이템<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 소울 크리스탈<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 보스전 스폰 검<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 보스전 불 액터<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 보스전 용 석상<br/>

<table style="text-align: center; margin: auto;">
  <tr>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/584809ae-fa0f-42f1-a62b-7e56e99005a1" width="100">
        <span style="margin-top: 8px;">경험치 구슬</span>
      </div>
    </td>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/e9ae91f6-f5ef-4206-87fd-533d9d5cac52" width="100">
        <span style="margin-top: 8px;">소울 크리스탈</span>
      </div>
    </td>
  </tr>
  <tr>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/569ae9a7-daba-41cd-b389-e5d04b4c55cd" width="100">
        <span style="margin-top: 8px;">확률성 아이템</span>
      </div>
    </td>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/5ed60976-7b67-4fae-a641-a0c196f7495e" width="100">
        <span style="margin-top: 8px;">확률성 아이템 </span>
      </div>
    </td>
  </tr>
  <tr>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; justify-content: flex-end; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/a4c683e0-e05e-4c21-96b0-a7931b23c295" width="100">
        <span style="margin-top: 8px;">보스전 스폰 검</span>
      </div>
    </td>
    <td style="height: 140px;">
      <div style="display: flex; flex-direction: column; align-items: center; justify-content: flex-end; min-height: 140px;">
        <img src="https://github.com/user-attachments/assets/ee6565ff-78f3-478d-800c-4f9cfb50171e" width="100">
        <span style="margin-top: 8px;">보스전 용 석상</span>
      </div>
    </td>
  </tr>
</table><br/>

> **Fracture Mesh**<br/>

![Image](https://github.com/user-attachments/assets/3e3203b2-71dd-47ea-b404-84e118c8f04e) <br/>
> **Demon Sword (보스맵 스폰 검) 이동 방식**<br/>

![Image](https://github.com/user-attachments/assets/40673ad6-2b22-4d41-a56f-93a291a882f5)<br/>
<details>
  <summary><strong>DemonSword.cpp 코드 보기</strong></summary>

  <pre style="max-height: 500px; overflow-y: auto; background-color: #1e1e1e; color: #d4d4d4; padding: 16px; border-radius: 8px; font-size: 14px;">
<code>
#include "DemonSword.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

ADemonSword::ADemonSword()
{
    PrimaryActorTick.bCanEverTick = true;

    swordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("swordComp"));
    sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
    damageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("damageBox"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> bossOBJ(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Boss_Alpernia/DemonSword/DemonSword.DemonSword'"));
    if (bossOBJ.Succeeded())
    {
        swordComp->SetStaticMesh(bossOBJ.Object);
    }
    SetRootComponent(sceneComp);
    swordComp->SetupAttachment(RootComponent);
    damageBox->SetupAttachment(RootComponent);
    swordComp->SetRelativeLocation(FVector(0, 0, 560));
    swordComp->SetRelativeRotation(FRotator(0, 180, 180));
    swordComp->SetWorldScale3D(FVector(3.0));
    damageBox->SetRelativeLocation(FVector(0, 0, 307));
    damageBox->SetRelativeRotation(FRotator(0, 0, 0));
    damageBox->SetWorldScale3D(FVector(1.75, 0.25, 7.0));
    swordComp->SetWorldScale3D(FVector(2.0));
    startLocation = swordComp->GetRelativeLocation();
    targetLocation = startLocation + FVector(0, 0, -200);
}

void ADemonSword::BeginPlay()
{
    Super::BeginPlay();
    upLoc = GetActorLocation() + FVector(0, 0, 800);
    direction = (upLoc - GetActorLocation()).GetSafeNormal();
    upNewLocation = GetActorLocation() + FVector(0, 0, 800);
    damageBox->OnComponentBeginOverlap.AddDynamic(this, &ADemonSword::OnBeginOverlapSwordFloor);
    swordCurrentHP = swordMaxHP;
}

void ADemonSword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    currentTime += DeltaTime;

    float adjustDuration = moveDuration / 3.2f;
    float alpha = FMath::Clamp(currentTime / adjustDuration, 0.0f, 1.0f);
    float easedAlpha = 1.0f - FMath::Pow(1.0f - alpha, 2.0f);
    FVector newLocation = FMath::Lerp(startLocation, targetLocation, easedAlpha);
    swordComp->SetRelativeLocation(newLocation);

    if (currentTime > 15.0f && currentTime <= 18.0f)
    {
        FVector upNewLocation2 = FMath::InterpEaseOut(GetActorLocation(), upNewLocation, (currentTime - 15) / 3 ,1.2f);
        SetActorLocation(upNewLocation2);

        if (!onceSound)
        {
            onceSound = true;
            int32 value = FMath::RandRange(1, 3);
            if (value == 1)
                UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound1, GetActorLocation());
            else if (value == 2)
                UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound2, GetActorLocation());
            else
                UGameplayStatics::PlaySoundAtLocation(this, UpSwordSound3, GetActorLocation());
        }
    }

    if (currentTime > 14.0f && !directionSet)
    {
        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

        int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
        player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
        directionSet = true;
        setplayerLoc = player->GetActorLocation();
        setDirection = player->GetActorLocation() - GetActorLocation();
    }

    if (currentTime > 15.5f && currentTime <= 17.0f)
    {
        FVector directionToPlayer = player->GetActorLocation() - GetActorLocation();
        FRotator baseRotation = FRotationMatrix::MakeFromZ(-directionToPlayer).Rotator();
        FQuat currentQuat = GetActorRotation().Quaternion();
        FQuat targetQuat = baseRotation.Quaternion();
        float lerpAlpha = FMath::Clamp((currentTime - 15.5f) / 2.5f, 0.0f, 1.0f);
        newQuat = FQuat::Slerp(currentQuat, targetQuat, lerpAlpha);
        SetActorRotation(newQuat);
    }

    if (currentTime > 18.0f && currentTime < 23.f)
    {
        FVector downDirection = -newQuat.GetUpVector();
        float speed = 9000.0f;
        FVector movement = downDirection * speed * DeltaTime;
        FVector stingLoc = GetActorLocation() + movement;
        SetActorLocation(stingLoc);
    }

    if (swordCurrentHP <= 0)
    {
        Destroy();
        onceSound = false;
        onceSound2 = false;
    }
}
</code>
  </pre>
</details>


## Crafting
크래프팅 시스템<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 데이터 테이블로 관리하는 쉬운 에셋추가 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 구조체를 사용한 쉬운 레시피, 수량 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Name위젯 , Recipe위젯  CraftingWidget의 값 제어 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 소켓에 따른 스냅방식 HUD <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 프리뷰 메시를 사용한 시각적 표현 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Folliage Actor의 별도 인스턴스화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Tag방식 <br/>



|   |   |   |
|---|---|---|
| ![첫 번째 이미지](https://github.com/user-attachments/assets/fb6f9afd-9fa6-4dd4-b329-b30efa331693) |  ➡️  | ![두 번째 이미지](https://github.com/user-attachments/assets/22e793f9-8c5b-4d7d-81cb-e34957802719) |

> **Item Structure**<br/>

![Image](https://github.com/user-attachments/assets/dbbe1d72-bcb0-4e4e-b180-da0c85012cec)<br/>
![Image](https://github.com/user-attachments/assets/ec505ee7-4c8e-4448-b3b2-49e4b52d151a)<br/>

## 프로젝트 목표<br/>
✅ 멀티플레이 환경구축<br/>
✅ BehaviorTree를 사용한 AI 제작<br/>
✅ 크래프팅 시스템의 구현<br/>
✅ 한사이클이 돌아가는 rpg 게임 구현<br/>



