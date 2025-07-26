// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "CharacterStatData.h"
#include "../Npc/shopItemData.h"
#include "../MainCharacter/MainWidget.h"
#include "../MainCharacter/SwordCharacter.h"
#include "StatGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOItemUpdate, const FString&, itemName, int32, itemCount);

UCLASS()
class BLASTERDREAM_API UStatGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	//�÷��̾� ó�� ����
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float playerSaveHp = 10000.0;
	float playerSaveMp = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float playerHP = 10000.0f;

	float playerMp = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float playerFullHp = 10000.0;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float playerFullMp = 5000.0f;

	float playerSpeed;
	float PlayerDefensivePower;
	int playerMoney;

	float userC = 0;



	FString UnitName;

	//save dragonforce
	bool instanceBGaze14 = false;
	float instanceCurrentSoulGaze;
	bool instanceBForceMode;
	float instanceCurrentForceGaze;
	bool instanceBCanPoly;
	
public:
	bool OpenShop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatData")
	UDataTable* characterStatTable;

	virtual void Init() override;

	const FCharacterStatData* GetCharacterStatID(FName rowname) const;

	float normalAttackDamage;
	float normalLastAttackDamage;
	float qAttackDamage;
	float qAttackLastDamage;
	float wAttackDamage;
	float wAttackLastDamage;
	float eAttackDamage;
	float eAttackLastDamage;
	float rAttackDamage;
	float rAttackLastDamage;

	float randomPlusNum;
	//Luco-----------------------------------
	UFUNCTION()
	void setDungeonSkeletonDieCountting();
	UFUNCTION()
	void setDungeonSkeletonAttackCounting();
	UFUNCTION()
	void setDungeonReviveCounting();
	int32 skeletonHitCounting = 0;
	int32 skeletonDieSumCount = 0;
	int32 reviveCount = 0;
	class APlayerController* playerCon;
	class ASwordCharacter* player;


	UFUNCTION()
	void renewalPlayerStat(float playerCurrentHpCopy, float playerCurrentMpCopy, float playerCurrentForceCopy, float playerMaxHpCopy, float playerMaxMpCopy, float playerMaxForceCopy);

	float playerCurrentHp;
	float playerCurrentMp;
	float playerCurrentForce;
	float playerMaxHp;
	float playerMaxMp;
	float playerMaxForce;
	int32 money;
	int32 hpPositionCount;
	int32 mpPositionCount;

	UFUNCTION()
	void ResetHpMp();

	//++ savefile
	UFUNCTION()
	void LoadPlayerData();

	UFUNCTION()
	void SavePlayerData(UWorld* world);

	UFUNCTION()
	void RespawnPlayerData();

	FVector saveLocation;
	FRotator saveRotator;
	float PlayercurrentHp;
	float PlayercurrentMp;
	bool bShouldRestoreTransform;


	//---------------------------------------

	//Dove_
	//�÷��̾� �̺�Ʈ 
	void setPlayerSetting(float& hp, float& mp);

	float setPlayerPower(float& playerPower);


	//ó�� �����Ҷ� �⺻ ���� 
	void GetPlayerStertSeting(float hp, float mp);
	//���Ϳ��� �¾����� �ߵ�__TODO(���� ����) Ȥ�� ����
	void GetHitPlayer(float hitDamage);



	//�κ� �ȿ� �ִ� ������ 
	UPROPERTY()
	TMap<FString, int32> ItemInventory;



	UPROPERTY()
	TMap<int32 ,FString> SlotItems;

	UPROPERTY()
	TMap<int32, FString> userGear;

	UPROPERTY()
	UMainWidget* mainWidget;

	ASwordCharacter* SwordPlayer;

	void InvenSlotSave();

	void userGearSlotSave();

	UFUNCTION()
	const TMap<int32 ,FString>& GetSlotItems() const;

	UFUNCTION()
	const TMap<int32, FString>& GetInstalItems() const;

	UFUNCTION(BlueprintCallable)
	bool checkItemCount(const FString& ItemName) const;

	FString CallItemImg(const FString& ItemName);

	void AddItem(FString itemname);

	void RemoveItem(FString itemname);

	void startItemes();

	void showShopUi(const FString& UnitTpye);


	//�κ��丮 ����Լ�
	void setInven(FString itemName);

	void removeInven(const FString& ItemName, const int32& itemCount);

	//UI ���� 
	void visbleUI(const FString& UIName);

	//�α׿��� Ȯ���ϱ� ���� �Լ�
	void PrintInven();

	void usingItem(const FString& useItemName);
	void usingHpPotion(float& hp, float HPpotion);
	void usingMpPotion(float& mp, float MPpotion);

	UPROPERTY(EditAnywhere, Category="Shop")
	USoundBase* itemBuySound;

	UPROPERTY(EditAnywhere, Category = "Shop")
	USoundBase* notEnoughMoneySound;

	void userInstallation(const FString& getItems, bool inItems);

	bool buyItemsMoney(const int32& price);
	void getMoney(const int32& getmoney);

	bool checkEquipment(const FString& SlotItemName);

	void GetRay(const FString& UintName);
	void tradeUnitInteraction();


	//��������Ʈ 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOItemUpdate OItemUpdate;

	void EmptyItems();

	UPROPERTY()
	UMainWidget* MainWidgetRef;

	UPROPERTY()
	UDataTable* ItemData;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FName targetLevelToLoad;

	//gameClear
	float totalClearTime = 0;

	UFUNCTION()
	void AddClearTime(float deltaSeconds);

	//�񵿱� �ε�
	UPROPERTY(VisibleAnywhere)
	FString lastTargetLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaPlayer* katanaMediaPlayer;

	UPROPERTY()
	class UMediaTexture* katanaMediaTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaSource* katanaMediaSource;

	UFUNCTION()
	void OnMediaOpened(FString OpenedUrl);

	UFUNCTION()
	void ResetMediaPlayer();

	bool bIsMediaPlaying = false;
};
