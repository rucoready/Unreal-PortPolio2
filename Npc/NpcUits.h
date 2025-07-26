// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CineCameraActor.h"
#include "NpcInteractionWidget.h"
#include "NpcUits.generated.h"

UCLASS()
class BLASTERDREAM_API ANpcUits : public ACharacter
{
	GENERATED_BODY()


public:
	ANpcUits();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* detectSphere;

	UPROPERTY(EditAnywhere, Category="Detect")
	float detectRadius = 200.0;

	UPROPERTY(EditAnywhere, Category = "Detect")
	UWidgetComponent* npcGWidgetInteractionComponent;

	class ASwordCharacter* player;

	UPROPERTY(EditAnywhere, Category = "Trader")
	FString NPCName;



	UFUNCTION()
	void ShowNpcWidget();

	UFUNCTION()
	void HideNpcWidget();

	class UStatGameInstance* gameInstance;
	

	UFUNCTION()
	void OnBeginOverlapNpc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapNpc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void StartInteractionCamera();

	UFUNCTION()
	void EndInteractionCamera();

	UPROPERTY()
	ACineCameraActor* npcInteractionCamera;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	FName npcCameraTagName;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	FString npcInteractionTalk;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UNpcInteractionWidget>interactionWidgetClass;

	UNpcInteractionWidget* interactionWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	UAnimMontage* talkMontage;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	UWidgetComponent* NamePlateWidget;


};
