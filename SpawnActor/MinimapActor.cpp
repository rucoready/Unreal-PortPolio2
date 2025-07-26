


#include "MinimapActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Engine/CanvasRenderTarget2D.h"



AMinimapActor::AMinimapActor()
{
	//minimap camera spring arm
	PrimaryActorTick.bCanEverTick = true;

    // SpringArmComponent 설정
    minimapCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArmComp"));
    minimapCameraBoom->SetupAttachment(RootComponent);
    minimapCameraBoom->TargetOffset = FVector(0.f, 0.f, 5000.f); // 위로 띄우기
    minimapCameraBoom->SetRelativeRotation(FRotator::ZeroRotator); // 절대 회전 없음

    minimapCameraBoom->bUsePawnControlRotation = false;
    minimapCameraBoom->bInheritPitch = false;
    minimapCameraBoom->bInheritRoll = false;
    minimapCameraBoom->bInheritYaw = false;

    // SceneCaptureComponent2D 설정
    minimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureMinimap"));
    minimapCapture->SetupAttachment(minimapCameraBoom);
    minimapCapture->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // 정확히 아래를 봄 (Pitch -90)
    minimapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
    minimapCapture->OrthoWidth = 1500.0f;

    static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> renderObj(TEXT("/Game/Luco/MiniMap/CRT_Minimap.CRT_Minimap"));
    if (renderObj.Succeeded())
    {
        minimapCapture->TextureTarget = renderObj.Object;
    }

    // Minimap Sprite 설정
    minimapSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
    minimapSprite->SetupAttachment(RootComponent);
    minimapSprite->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, -90.f))); // 위에서 봐도 보이게
    minimapSprite->SetRelativeScale3D(FVector(0.5f));
    minimapSprite->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
    minimapSprite->bVisibleInSceneCaptureOnly = true;
}


void AMinimapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMinimapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

