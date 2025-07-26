// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnWidget.h"
#include "../../MainCharacter/SwordPlayController.h"
#include "../../GameManager/StatGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URespawnWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(DieLogoAnim);
	Button_Respawn->SetVisibility(ESlateVisibility::Collapsed);
	if (Button_Respawn)
	{
		Button_Respawn->OnClicked.AddDynamic(this, &URespawnWidget::ClickRespawnButton);
		Button_Respawn->OnHovered.AddDynamic(this, &URespawnWidget::HoverRespawnButton);
		Button_Respawn->OnUnhovered.AddDynamic(this, &URespawnWidget::UnhoverRespawnButton);
	}
	UpdateTimerText();
	UStatGameInstance* myInstance = Cast<UStatGameInstance>(GetGameInstance());
	myInstance->RespawnPlayerData();
	GetWorld()->GetTimerManager().SetTimer(th_showButton, this, &URespawnWidget::CountdownText, 1.0f, true);
}

void URespawnWidget::ShowButton()
{
	Button_Respawn->SetVisibility(ESlateVisibility::Visible);
}

void URespawnWidget::ClickRespawnButton()
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (pc)
	{
		ASwordPlayController* swordController = Cast<ASwordPlayController>(pc);
		swordController->SpawnCharacter();
		this->RemoveFromParent();
	}
}

void URespawnWidget::HoverRespawnButton()
{
	PlayAnimation(RespawnHoverAnim);
}

void URespawnWidget::UnhoverRespawnButton()
{
	PlayAnimation(RespawnUnHoverAnim);
}

void URespawnWidget::UpdateTimerText()
{
	if (TextBlock_TimerText)
	{
		FString customStr = FString::FromInt(countValue) + TEXT("초 뒤 부활합니다");
		TextBlock_TimerText->SetText(FText::FromString(customStr));
	}
}

void URespawnWidget::CountdownText()
{
	countValue--;
	if (countValue == 0)
	{
		TextBlock_TimerText->SetVisibility(ESlateVisibility::Collapsed);
		ShowButton();
		GetWorld()->GetTimerManager().ClearTimer(th_showButton);
	}
	else
	{
		
		UpdateTimerText();
	}
}
