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

## Boss
던전 마지막에 배치된 복잡한 AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossWidget 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● BossBlackBoard 관리 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 네트워크 동기화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Service Node, Decorator Node를 사용한 여러가지 조건 체크<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● Blackboard과 BehaviorTree의 상호작용<br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 각각 Task에서 독립적인 패턴 구현<br/>

> **Boss 설계도**<br/>

![Image](https://github.com/user-attachments/assets/b17883c1-38a0-46f5-8e1f-eea31f9bd5ad)<br/>

| Phase1 Nodes | Phase2 Nodes |
|:--:|:--:|
| ![Phase1](https://github.com/user-attachments/assets/b2c2124e-21c3-4aa2-9dfe-5cccb43684e6)<br/><sub></sub> | ![Phase2](https://github.com/user-attachments/assets/cfef0d85-6d41-4863-98f6-33b1b51c47c4)<br/><sub></sub> |

| Pattern Nodes | All Nodes |
|:--:|:--:|
| ![Phase3](https://github.com/user-attachments/assets/661f0aaf-756f-4a8a-81f1-82f447ea6222)<br/><sub></sub> | ![Phase4](https://github.com/user-attachments/assets/088dfdb6-a959-4a48-9530-d5d0f2578e5d)<br/><sub></sub> |<br/>
> **Service_Calculate Distance Node**
```cpp
#include "Boss/Service_CalculateDistanceBetween.h"
#include "Player/PixelCodeCharacter.h"
#include "Boss/BossApernia.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

UService_CalculateDistanceBetween::UService_CalculateDistanceBetween()
{
    NodeName = TEXT("Calculate Between Boss To Player");
    currentTime = 0.0f;
    timeToSelectPlayer = 10.0f; // 플레이어를 선택할 주기
}

void UService_CalculateDistanceBetween::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    currentTime += DeltaSeconds;

    // BossApernia 찾기
    ABossApernia* bossCharacter = Cast<ABossApernia>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossApernia::StaticClass()));
    if (!bossCharacter)
    {
        return;
    }
    if (currentTime >= timeToSelectPlayer)
    {
        currentTime = 0.0f;
        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

        if (foundCharacters.Num() > 0)
        {
            int32 randomIndex = FMath::RandRange(0, foundCharacters.Num() - 1);
            player = Cast<APixelCodeCharacter>(foundCharacters[randomIndex]);
            if (player)
            {
                FVector playerLocation = player->GetActorLocation();
            }
        }
    }
    if (player)
    {
        float distance = FVector::Distance(bossCharacter->GetActorLocation(), player->GetActorLocation());
        OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), distance);
    }
}
```

## Enemy AI
맵에 배치된 기본적인 적AI<br/>

&nbsp;&nbsp;&nbsp;&nbsp;● BehaviorTree 동작방식 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● StateMachine 상반신 하반신 개별화 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 독자적 개인 시야각 적용 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 데미지 출력 HUD <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 여러가지 아이템 확률드랍 <br/>
&nbsp;&nbsp;&nbsp;&nbsp;● 네트워크 동기화 <br/>
| Grux | Dog |
|:--:|:--:|
| <div align="center"> ![왼쪽 위](https://github.com/user-attachments/assets/df94cdcc-cfee-4583-9248-461bde2f5472) </div> | <div align="center"> ![오른쪽 위](https://github.com/user-attachments/assets/bbd4e4be-a4dd-4b1b-ad49-3ab117255f86) </div> |
| <div align="center"> ![왼쪽 아래](https://github.com/user-attachments/assets/4eaa73d2-e43c-45a0-a112-fc89318e229a) </div> | <div align="center"> ![오른쪽 아래](https://github.com/user-attachments/assets/8a78faf6-cc62-4232-8241-b12da5d8bef6) </div> |

> **Task_DogBartMoveToPlayer Node**
```cpp
#include "Task_DogBartMoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DogBartAIController.h"
#include "DogBart.h"
#include "NavigationSystem.h"


UTask_DogBartMoveToPlayer::UTask_DogBartMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To nearest Player");
}

EBTNodeResult::Type UTask_DogBartMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (BlackboardComp)
    {
        // 블랙보드에서 벡터 값을 가져옵니다.
        FVector NearestPlayerLocation = BlackboardComp->GetValueAsVector(TEXT("DetectedPlayerLoc"));

        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            ADogBartAIController* dogBartController = Cast<ADogBartAIController>(OwnerComp.GetAIOwner());
            if (dogBartController)
            {
                
                dogBartController->MoveToLocation(NearestPlayerLocation);
            }
        }      
    }
    return EBTNodeResult::Failed;
}
```

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



