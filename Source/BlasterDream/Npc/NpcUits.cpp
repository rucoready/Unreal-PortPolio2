
#include "NpcUits.h"
#include "Engine/EngineTypes.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "EngineUtils.h"
#include "../MainCharacter/SwordPlayController.h"
#include "NpcInteractionWidget.h"
#include "../GameManager/StatGameInstance.h"



ANpcUits::ANpcUits()
{
	PrimaryActorTick.bCanEverTick = true;

	detectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("detectSphere"));
	detectSphere->SetupAttachment(RootComponent);
	detectSphere->SetSphereRadius(detectRadius);
	detectSphere->SetCollisionProfileName(TEXT("npcInterectionTrigget"));

	npcGWidgetInteractionComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("GInteractionComponent"));
	npcGWidgetInteractionComponent->SetupAttachment(RootComponent);
	npcGWidgetInteractionComponent->SetRelativeLocation(FVector(52.f, 0.f, 52.f));
	npcGWidgetInteractionComponent->SetWidgetSpace(EWidgetSpace::Screen);
	npcGWidgetInteractionComponent->SetVisibility(false);


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	NamePlateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlate"));
	NamePlateWidget->SetupAttachment(RootComponent);
	NamePlateWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NamePlateWidget->SetRelativeLocation(FVector(0.f, 0.f, 130.f));


}

void ANpcUits::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<UStatGameInstance>(UGameplayStatics::GetGameInstance(this));
	detectSphere->OnComponentBeginOverlap.AddDynamic(this, &ANpcUits::OnBeginOverlapNpc);
	detectSphere->OnComponentEndOverlap.AddDynamic(this, &ANpcUits::EndOverlapNpc);
	for (TActorIterator<ACineCameraActor>it(GetWorld()); it; ++it)
	{
		ACineCameraActor* npcCineCam = *it;
		if (npcCineCam->ActorHasTag(npcCameraTagName))
		{
			npcInteractionCamera = npcCineCam;
		}
	}
}

void ANpcUits::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANpcUits::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ANpcUits::StartInteractionCamera()
{
	player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GetMesh()->SetVisibility(false);
	player->overlapSword->SetVisibility(false);

	APlayerController* pc = Cast<APlayerController>(player->GetController());
	ASwordPlayController* swordCon = Cast<ASwordPlayController>(pc);
	if (player)
	{

		swordCon->SetViewTargetWithBlend(npcInteractionCamera, 1.f);
		
		swordCon->InteractionHidePlayerWidget();
	}
	
	npcGWidgetInteractionComponent->SetVisibility(false);
	gameInstance->MainWidgetRef->BP_ItemWidget->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle th_ShowNpcWidget;
	GetWorldTimerManager().SetTimer(th_ShowNpcWidget, this, &ANpcUits::ShowNpcWidget, 1.0f, false);

	NamePlateWidget->SetVisibility(false);
}

void ANpcUits::EndInteractionCamera()
{
	player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GetMesh()->SetVisibility(true);
	if (player->bHasAttackMode)
	{
		player->overlapSword->SetVisibility(true);
	}
	
	APlayerController* pc = Cast<APlayerController>(player->GetController());
	ASwordPlayController* swordCon = Cast<ASwordPlayController>(pc);
	if (player)
	{
		swordCon->SetViewTargetWithBlend(player, 1.0f);
		swordCon->InteractionShowPlayerWidget();
	}
	npcGWidgetInteractionComponent->SetVisibility(true);
	HideNpcWidget();
	NamePlateWidget->SetVisibility(true);
	player->equipmentUI->BP_ItemWidget->SetVisibility(ESlateVisibility::Visible);

}

void ANpcUits::ShowNpcWidget()
{
	if (gameInstance)
	{
		//ui 띄우기
		gameInstance->showShopUi(NPCName);
		gameInstance->visbleUI(TEXT("Inven"));
	}
	interactionWidgetInstance = CreateWidget<UNpcInteractionWidget>(GetWorld(), interactionWidgetClass);
	interactionWidgetInstance->AddToViewport(6);
	interactionWidgetInstance->SetInteractionText(npcInteractionTalk);
	interactionWidgetInstance->SetOwnNpc(this);
	PlayAnimMontage(talkMontage);
	
}

void ANpcUits::HideNpcWidget()
{
	if (gameInstance)
	{
		//ui 내리기
		gameInstance->visbleUI(FString("Shop"));
		gameInstance->visbleUI(FString("Inven"));
	}

}

void ANpcUits::OnBeginOverlapNpc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (OtherActor == player)
	{
		npcGWidgetInteractionComponent->SetVisibility(true);
		player->currentInteractionNpc = this;
	}
}

void ANpcUits::EndOverlapNpc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == player)
	{
		npcGWidgetInteractionComponent->SetVisibility(false);
		player->currentInteractionNpc = nullptr;
	}
}






