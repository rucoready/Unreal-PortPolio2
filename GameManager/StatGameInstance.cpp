// Fill out your copyright notice in the Description page of Project Settings.


#include "StatGameInstance.h"
#include "../Npc/shopItemData.h"
#include "../MainCharacter/MainWidget.h"
#include "../Npc/itemDataTable.h"
#include "../MainCharacter/SwordCharacter.h"
#include "Engine/DataTable.h"
#include "../MainCharacter/SwordCharacter.h"
#include "ProjectSaveGame.h"
#include "MediaSource.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "Kismet/GameplayStatics.h"

UPROPERTY()
class UMainWidget* MainWidgetRef;

void UStatGameInstance::Init()
{
	Super::Init();

	const FCharacterStatData* statData = GetCharacterStatID(TEXT("Skill"));
	if (statData)
	{
		//damage Stat calculate
		randomPlusNum = FMath::RandRange(5, 20);
		normalAttackDamage = statData->normalAttackBaseDamage;
		normalLastAttackDamage = statData->normalAttackLastBaseDamage;
		qAttackDamage = statData->skillQBaseDamage;
		qAttackLastDamage = statData->skillQLastBaseDamage;
		wAttackDamage = statData->skillWBaseDamage;
		wAttackLastDamage = statData->skillWLastBaseDamage;
		eAttackDamage = statData->skillEBaseDamage;
		eAttackLastDamage = statData->skillELastBaseDamage;
		rAttackDamage = statData->skillRBaseDamage;
		rAttackLastDamage = statData->skillRLastBaseDamage;
	}


	ItemData = LoadObject<UDataTable>(
		nullptr,
		TEXT("/Game/DoveDove/DB_ItemDataTable.DB_ItemDataTable")
	);

	if (!katanaMediaPlayer)
	{
		katanaMediaPlayer = NewObject<UMediaPlayer>(this);
	}

	if (!katanaMediaTexture)
	{
		katanaMediaTexture = NewObject<UMediaTexture>(this);
		katanaMediaTexture->SetMediaPlayer(katanaMediaPlayer);
		katanaMediaTexture->UpdateResource();
	}
	if (katanaMediaSource)
	{
		if (katanaMediaPlayer->OpenSource(katanaMediaSource))
		{
			katanaMediaPlayer->Play();
			katanaMediaPlayer->Pause(); 
		}
	}
	
	
}

const FCharacterStatData* UStatGameInstance::GetCharacterStatID(FName rowname) const
{
	return characterStatTable->FindRow<FCharacterStatData>(rowname, TEXT(""));
}

//Luco--------------------------------------------------------
void UStatGameInstance::setDungeonSkeletonDieCountting()
{
	skeletonDieSumCount++;
}

void UStatGameInstance::setDungeonSkeletonAttackCounting()
{
	skeletonHitCounting++;
}

void UStatGameInstance::setDungeonReviveCounting()
{
	reviveCount++;
}

void UStatGameInstance::renewalPlayerStat(float playerCurrentHpCopy, float playerCurrentMpCopy, float playerCurrentForceCopy, float playerMaxHpCopy, float playerMaxMpCopy, float playerMaxForceCopy)
{
	playerCurrentHp = playerCurrentHpCopy;
	playerCurrentMp = playerCurrentMpCopy;
	playerCurrentForce = playerCurrentForceCopy;
	playerMaxHp = playerMaxHpCopy;
	playerMaxMp = playerMaxMpCopy;
	playerMaxForce = playerMaxForceCopy;
}

void UStatGameInstance::AddClearTime(float deltaSeconds)
{
	totalClearTime += deltaSeconds;
}

void UStatGameInstance::OnMediaOpened(FString OpenedUrl)
{
	if (katanaMediaPlayer->Play())
	{
		bIsMediaPlaying = true;
	}
	else
	{
		bIsMediaPlaying = false;
	}
}

void UStatGameInstance::ResetMediaPlayer()
{
	if (katanaMediaPlayer)
	{
		if (katanaMediaPlayer->IsPlaying())
		{
			katanaMediaPlayer->Pause();  
		}
		katanaMediaPlayer->Close(); 
	}
}



void UStatGameInstance::ResetHpMp()
{
	playerSaveHp = playerFullHp;
	playerSaveMp = playerFullMp;
}
void UStatGameInstance::LoadPlayerData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveData"), 0))
	{
		UProjectSaveGame* loadedData = Cast<UProjectSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveData"), 0));
		if (loadedData)
		{
			FString currentMapName = GetWorld()->GetMapName();
			currentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			//저장맵이 지금맵이랑 다르면 오픈
			if (currentMapName != loadedData->lastMapName)
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(*loadedData->lastMapName));
				return;
			}
			else
			{
				saveLocation = loadedData->playerLastLocation;
				saveRotator = loadedData->playerLastRotation;
				bShouldRestoreTransform = true;
			}
		}
	}
}
//세이브파일에 변수값들을 저장하면서 .sav파일을 만드는함수
void UStatGameInstance::SavePlayerData(UWorld* world)
{
	ACharacter* pawn = UGameplayStatics::GetPlayerCharacter(world, 0);
	ASwordCharacter* playerCharacter = Cast<ASwordCharacter>(pawn);
	UProjectSaveGame* saveGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));

	//원래되던것
	//UProjectSaveGame* saveGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));
	//player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	


	if (saveGameInstance)
	{
		//save data
		saveGameInstance->saveSlotName = "PlayerSaveData";
		saveGameInstance->saveIndex = 0;

		saveGameInstance->savePlayerHp = playerSaveHp;
		saveGameInstance->savePlayerMp = playerSaveMp;

		//loc, rot, map
		saveGameInstance->playerLastLocation = playerCharacter->GetActorLocation();
		saveGameInstance->playerLastRotation = playerCharacter->GetActorRotation();
		FString mapName = GetWorld()->GetMapName();

		//items
		saveGameInstance->userC = userC;
		saveGameInstance->playerMoney = playerMoney;

		InvenSlotSave();
		userGearSlotSave();
	
		saveGameInstance->userGear = userGear;
		saveGameInstance->slotItems = SlotItems;
		saveGameInstance->itemInventory = ItemInventory;


		mapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		saveGameInstance->lastMapName = mapName;

		//bcanpoly
		saveGameInstance->saveBCanPoly = instanceBCanPoly;

		saveGameInstance->saveBGaze14 = instanceBGaze14;
		saveGameInstance->saveCurrentSoulGaze = instanceCurrentSoulGaze;
		UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->saveIndex);
	}
}
void UStatGameInstance::RespawnPlayerData()
{
	UProjectSaveGame* saveGameInstance = Cast<UProjectSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectSaveGame::StaticClass()));
	player = Cast<ASwordCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));


	if (saveGameInstance)
	{
		//save data
		saveGameInstance->saveSlotName = "PlayerSaveData";
		saveGameInstance->saveIndex = 0;

		saveGameInstance->savePlayerHp = 10000;
		saveGameInstance->savePlayerMp = 5000;
		UGameplayStatics::SaveGameToSlot(saveGameInstance, saveGameInstance->saveSlotName, saveGameInstance->saveIndex);
	}
}
//--------------------------------------------------------------

void UStatGameInstance::setPlayerSetting(float& hp, float& mp)
{
	hp = playerHP;
	mp = playerMp;
}




void UStatGameInstance::GetPlayerStertSeting(float hp, float mp)
{
	playerHP = hp;
	playerMp = mp;
	//playerSpeed = speed;
}



void UStatGameInstance::AddItem(FString itemname)
{

	if (int32* ItemCount = ItemInventory.Find(itemname))
	{
		(*ItemCount)++;  // 값 증가
		MainWidgetRef->InvenInCount(itemname, ItemInventory[itemname]);
		OItemUpdate.Broadcast(itemname, ItemInventory[itemname]);
	}
	else
	{
		ItemInventory.Add(itemname, 1);
		OItemUpdate.Broadcast(itemname, 1);
		setInven(itemname);
	}
}

void UStatGameInstance::RemoveItem(FString itemname)
{
	if (int32* ItemCount = ItemInventory.Find(itemname))
	{
		if (*ItemCount != 0)
		{
			(*ItemCount)--;
			if (*ItemCount == 0)
			{
				ItemInventory.Remove(itemname);
				//OItemUpdate.Broadcast(itemname, 0); 
			}
			else
			{
				MainWidgetRef->InvenInCount(itemname, ItemInventory[itemname]);
			}

			OItemUpdate.Broadcast(itemname, ItemInventory.FindRef(itemname));
			removeInven(itemname, *ItemCount);
		}
	}

}

//시작할때 물약 등록 함수
void UStatGameInstance::startItemes()
{
	AddItem(FString(TEXT("HP_potion")));
	AddItem(FString(TEXT("MP_potion")));
}



void UStatGameInstance::showShopUi(const FString& UnitTpye)
{
	OpenShop = true;
	MainWidgetRef->OpenShop();
	MainWidgetRef->BP_ItemWidget->SetVisibility(ESlateVisibility::Hidden);
	if (MainWidgetRef)
	{
		TMap<FName, uint8*> RowMap = ItemData->GetRowMap();

		for (const TPair<FName, uint8*>& Pair : RowMap)
		{
			FName RowName = Pair.Key;
			FItemData* RowData = reinterpret_cast<FItemData*>(Pair.Value);

			if (RowData)
			{
				if(*RowData->salesperson == UnitTpye)		
					MainWidgetRef->showShopBox(*RowData->ItemName, RowData->Price, *RowData->itemImagePath);
			}
		}
		
	}
}




void UStatGameInstance::setInven(FString itemName)
{
	FName RowName = FName(*itemName);
	const FItemData* FoundRow = ItemData->FindRow<FItemData>(RowName, TEXT("FindItemData"));

	if (!FoundRow)
	{
		return;
	}

	if (!MainWidgetRef || !MainWidgetRef->bIsInitialized)
	{

		FTimerHandle TimerHandle;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(TimerHandle, [this, FoundRow]()
				{
					if (MainWidgetRef && MainWidgetRef->bIsInitialized)
					{
						MainWidgetRef->InvenIn(*FoundRow->ItemName, *FoundRow->itemImagePath);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Delayed InvenIn failed: MainWidget still not ready"));
					}
				}, 0.2f, false);
		}
		return;
	}

	// 정상 실행
	MainWidgetRef->InvenIn(*FoundRow->ItemName, *FoundRow->itemImagePath);

}

void UStatGameInstance::removeInven(const FString& ItemName, const int32& itemCount)
{
	MainWidgetRef->InvenRemove(ItemName, itemCount);
}

void UStatGameInstance::visbleUI(const FString& UIName)
{
	ACharacter* pawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASwordCharacter* playerCharacter = Cast<ASwordCharacter>(pawn);
	//MainWidgetRef->BP_ItemWidget->SetVisibility(ESlateVisibility::Visible);
	if (UIName == TEXT("Inven"))
		MainWidgetRef->visbleInven();
	else if(UIName == TEXT("UserInstal"))
		MainWidgetRef->visbleUserInstal();
	else if (UIName == TEXT("Shop"))
	{
		if (!OpenShop)
		{
			tradeUnitInteraction();
			OpenShop = true;
		}
		else
		{
			MainWidgetRef->closeShop();
			OpenShop = false;
		}
	}
	
}


void UStatGameInstance::PrintInven()
{
	//UE_LOG(LogTemp, Warning, TEXT("=== Inventory ==="));
	//for (const TPair<FString, int32>& Pair : ItemInventory)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Item: %s, Count: %d"), *Pair.Key, Pair.Value);
	//}
}

void UStatGameInstance::usingItem(const FString& useItemName)
{
	if (IsValid(SwordPlayer) && checkItemCount(useItemName))
	{
		SwordPlayer->usingItems(useItemName);
		RemoveItem(useItemName);

	}
	else
		return;


}


void UStatGameInstance::userInstallation(const FString& getItems, bool inItems)
{
	FName RowName = FName(*getItems);
	const FItemData* FoundRow = ItemData->FindRow<FItemData>(RowName, TEXT("FindItemData"));
	if (inItems)
		userC += FoundRow->Probability;
	else
		userC -= FoundRow->Probability;
	SwordPlayer->critsPercent = userC;
}

bool UStatGameInstance::buyItemsMoney(const int32& price)
{

	if (playerMoney < price)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), notEnoughMoneySound);
		return false;
	}
	else
	{
		playerMoney -= price;
		MainWidgetRef->changeMoney(playerMoney);
		UGameplayStatics::PlaySound2D(GetWorld(), itemBuySound);
		return true;
	}
	

}

void UStatGameInstance::getMoney(const int32& getmoney)
{
	playerMoney += getmoney;
	MainWidgetRef->changeMoney(playerMoney);
}

bool UStatGameInstance::checkEquipment(const FString& SlotItemName)
{
	FName RowName = FName(*SlotItemName);
	const FItemData* FoundRow = ItemData->FindRow<FItemData>(RowName, TEXT("FindItemData"));
	if(FoundRow->ItemType == TEXT("Item"))
		return false;
	return true;

}

void UStatGameInstance::GetRay(const FString& UintName)
{
	UnitName = UintName;
	MainWidgetRef->visblegKey(true);
}

void UStatGameInstance::tradeUnitInteraction()
{
	if (UnitName != TEXT(""))
	{
		showShopUi(UnitName);
		UnitName = TEXT("");
	}
}


void UStatGameInstance::EmptyItems()
{
	ItemInventory.Empty();

}


// 인벤 저장(아이템 위치) 
void UStatGameInstance::InvenSlotSave()
{
	SlotItems = MainWidgetRef->SlotSave();

}
//장착 장비 저장
void UStatGameInstance::userGearSlotSave()
{
	userGear = MainWidgetRef->InstalSlotSave();
}


const TMap<int32, FString>& UStatGameInstance::GetSlotItems() const
{
	return SlotItems;

}

const TMap<int32, FString>& UStatGameInstance::GetInstalItems() const
{
	return userGear;
}

FString UStatGameInstance::CallItemImg(const FString& ItemName)
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemData DataTable is nullptr!"));
		return TEXT(""); // 또는 기본 경로
	}

	FName RowName = FName(*ItemName);
	const FItemData* FoundRow = ItemData->FindRow<FItemData>(RowName, TEXT("FindItemData"));

	if (!FoundRow)
	{
		return TEXT("/Script/Engine.Texture2D'/Game/DoveDove/UI/Img/NULLimg.NULLimg'"); 
	}

	return FoundRow->itemImagePath;
}

bool UStatGameInstance::checkItemCount(const FString& ItemName) const
{
	const int32* ItemCount = ItemInventory.Find(ItemName);
	return ItemCount && *ItemCount > 0;
}

