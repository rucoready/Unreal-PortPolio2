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
  <h2 style="text-align: center; font-weight: bold;">Action Mapping을 사용한 액션바인드</h2>
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

> **Behavior Tree**
아래 이미지는 Behavior Tree의 전체 구조와 실행 흐름을 시각적으로 보여줍니다.

<div style="border: 1px solid #ccc; border-radius: 8px; padding: 16px; background-color: #f9f9f9;">
  <p><strong>🟦 왼쪽:</strong> 기본적인 트리 구조 &nbsp;&nbsp;&nbsp; <strong>🟩 오른쪽:</strong> 실행 흐름 다이어그램</p>
  <div style="display: flex; justify-content: space-between; flex-wrap: wrap;">
    <img src="https://github.com/user-attachments/assets/5566b686-4456-4c66-922a-51697a04f943" alt="Behavior Tree Left" style="width: 49%; border: 1px solid #aaa; border-radius: 6px;" />
    <img src="https://github.com/user-attachments/assets/d854d143-313e-4efa-ac8c-8327227b4cc5" alt="Behavior Tree Right" style="width: 49%; border: 1px solid #aaa; border-radius: 6px;" />
  </div>
</div>

> **Task_DogBartPatrol Node**
```cpp
#include "Task_DogBartPatrol.h"
#include "DogBartAIController.h"
#include "NavigationPath.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UTask_DogBartPatrol::UTask_DogBartPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("DogBart Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_DogBartPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TickTask(OwnerComp, NodeMemory, 0.0f);
	ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());
	if (dogBartController)
	{
		FVector origin = dogBartController->GetPawn()->GetActorLocation();
		navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
		FVector randomLocation;
		if (navSys)
		{
			if (navSys->K2_GetRandomReachablePointInRadius(GetWorld(), origin, randomLocation, 1500.0))
			{
				dogBartController->MoveToLocation(randomLocation);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

void UTask_DogBartPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	currentTime += DeltaSeconds;
}
```

> **StateMachine**<br/>

![Image](https://github.com/user-attachments/assets/a255c6c9-c8b6-4ad0-b2cc-7c8a914b5bb9) </div>

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



