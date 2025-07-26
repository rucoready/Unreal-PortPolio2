


#include "MinimapActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Engine/CanvasRenderTarget2D.h"



AMinimapActor::AMinimapActor()
{
	//minimap camera spring arm
	PrimaryActorTick.bCanEverTick = true;

    // SpringArmComponent ����
    minimapCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArmComp"));
    minimapCameraBoom->SetupAttachment(RootComponent);
    minimapCameraBoom->TargetOffset = FVector(0.f, 0.f, 5000.f); // ���� ����
    minimapCameraBoom->SetRelativeRotation(FRotator::ZeroRotator); // ���� ȸ�� ����

    minimapCameraBoom->bUsePawnControlRotation = false;
    minimapCameraBoom->bInheritPitch = false;
    minimapCameraBoom->bInheritRoll = false;
    minimapCameraBoom->bInheritYaw = false;

    // SceneCaptureComponent2D ����
    minimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureMinimap"));
    minimapCapture->SetupAttachment(minimapCameraBoom);
    minimapCapture->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // ��Ȯ�� �Ʒ��� �� (Pitch -90)
    minimapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
    minimapCapture->OrthoWidth = 1500.0f;

    static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> renderObj(TEXT("/Game/Luco/MiniMap/CRT_Minimap.CRT_Minimap"));
    if (renderObj.Succeeded())
    {
        minimapCapture->TextureTarget = renderObj.Object;
    }

    // Minimap Sprite ����
    minimapSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
    minimapSprite->SetupAttachment(RootComponent);
    minimapSprite->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, -90.f))); // ������ ���� ���̰�
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

