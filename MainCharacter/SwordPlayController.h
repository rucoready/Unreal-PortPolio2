
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SwordCharacter.h"
#include "../MainCharacter/CharacterWidget/RespawnWidget.h"
#include "MainWidget.h"
#include "SwordPlayController.generated.h"

/**
 *
 */
UCLASS()
class BLASTERDREAM_API ASwordPlayController : public APlayerController
{
	GENERATED_BODY()

public:
	ASwordPlayController();

	//SaveTransfrom
	virtual void OnPossess(APawn* myPawnPlayer) override;

	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite)
	bool bclickRightMouse;

	UFUNCTION()
	void InputMouseRightPressed();

	UFUNCTION()
	void InputMouseRightReleased();

	UFUNCTION()
	void InputTab();

	UFUNCTION()
	void InputMouseLeftPressed();

	UFUNCTION()
	void InputMouseLeftReleased();

	//QWERASDF
	UFUNCTION()
	void InputKeyboardQPressed();
	UFUNCTION()
	void InputKeyboardWPressed();
	UFUNCTION()
	void InputKeyboardEPressed();
	UFUNCTION()
	void InputKeyboardRPressed();
	UFUNCTION()
	void InputKeyboardRReleased();

	UFUNCTION()
	void InputKeyboardAPressed();
	UFUNCTION()
	void InputKeyboardSPressed();
	UFUNCTION()
	void InputKeyboardDPressed();
	UFUNCTION()
	void InputKeyboardFPressed();

	UFUNCTION()
	void InputKeyboard0Pressed();
	UFUNCTION()
	void InputKeyboard1Pressed();
	UFUNCTION()
	void InputKeyboard2Pressed();

	UFUNCTION()
	void InputKeyboardIPressed();
	UFUNCTION()
	void InputKeyboardPPressed();
	UFUNCTION()
	void InputKeyboardGPressed();
	UFUNCTION()
	void InputKeyboardEscPressed();
	UFUNCTION()
	void InputKeyboardCPressed();


	//left control
	UFUNCTION()
	void InputKeyboardCtrlPressed();

	bool bClickLeftMouseClick;

	UFUNCTION()
	void SetNewDestination(const FVector Destination);

	UFUNCTION()
	void MoveToMouseCursor();

	APawn* myPawn;

	UPROPERTY()
	class ASwordCharacter* swordCharacter;

	UPROPERTY()
	USwordCharacterWidget* swordCharacterWidget;

	FHitResult Hit;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> attackAnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> originAnimInstance;

	float lastClickTime = 0.0f;
	const float clickInterval = 0.2f;

	bool bLeftMouseClickedOnce;

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;
	float reloadCurrentTime;

	float lastAttackTime = 0.0f; // 마지막 공격 시간
	float attackCooldown = 1.0f; // 공격 쿨타임 (0.5초)

	float mouseRightClickAndLeftattackDelay = 0.3f;

	UPROPERTY()
	UAnimInstance* animInstance;

	void getMousePosition();
	UFUNCTION()
	void getCurrentActorLocAndRot();

	FVector currentPlayerLoc;
	FRotator currentPlayerRot;

	FHitResult skillMouseCursorHit;
	bool bClickRHolding;

	//ASDF

	//A

	UFUNCTION()
	void CheckMouseCursorShape();

	bool bOnMouseCursorRecord;

	UFUNCTION()
	void BlockingSkillKey();

	UFUNCTION()
	void BlockKey();

	class ASwordPlayerGameBase* gameMode;

	//instance caching
	class UStatGameInstance* myInstance;

	//Respawn
	UFUNCTION()
	void RevivePlayer(ASwordCharacter* dyingCharacter);

	UFUNCTION()
	void SpawnCharacter();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	TSubclassOf<ASwordCharacter> respawnCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Respawn")
	TSubclassOf<URespawnWidget> respawnWidget;

	FVector playerSpawnLoc;
	FRotator playerSpawnRot;

	UFUNCTION()
	void grayColorGradingSet();

	UFUNCTION()
	void grayColorGradingUnSet();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	UNiagaraSystem* respawnNa;

	//UI 띄우기용

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;

	class UMainWidget* eqipmentWidget;
	class UCharacterWidget* characterWidget;

	bool bPlayerDie = false;

	//dragon poly
	UPROPERTY()
	bool bCanPoly = false;

	UFUNCTION()
	void InteractionHidePlayerWidget();

	UFUNCTION()
	void InteractionShowPlayerWidget();

	ASwordCharacter* mySwordPlayer;


};
