// Fill out your copyright notice in the Description page of Project Settings.


#include "DragonController.h"
#include "DragonCharacter.h"
#include "DrangonAnimInstance.h"
#include "../MainCharacter/CharacterWidget/DragonWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void ADragonController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADragonController::OnPossess(APawn* myPawnPlayer)
{
	Super::OnPossess(myPawnPlayer);
	dragonCharacter = Cast<ADragonCharacter>(myPawnPlayer);
	dragonCharacter->PossessSetting();
	pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//mouse setting
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	EnableInput(this);
	//InputComponent->BindAction("RightClick", IE_Pressed, this, &ADragonController::InputMouseRightPressed);
	//InputComponent->BindAction("RightClick", IE_Released, this, &ADragonController::InputMouseRightReleased);
	//InputComponent->BindAction("KeyboardSpace", IE_Released, this, &ADragonController::InputMouseSpacePressed);
	//InputComponent->BindAction("KeyboardQ", IE_Released, this, &ADragonController::InputQPressed);
	//InputComponent->BindAction("KeyboardW", IE_Pressed, this, &ADragonController::InputWPressed);
	//InputComponent->BindAction("KeyboardW", IE_Released, this, &ADragonController::InputWUnPressed);

	if (dragonCharacter)
	{
		USkeletalMeshComponent* mesh = dragonCharacter->GetMesh();
		if (mesh)
		{
			dragonInstance = Cast<UDrangonAnimInstance>(mesh->GetAnimInstance());
		}
	}
	
}

void ADragonController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bclickRightMouse)
	{
		MoveToMouseCursor();

	}
	CheckMouseCursorShape();
	
	
}

void ADragonController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("RightClick", IE_Pressed, this, &ADragonController::InputMouseRightPressed);
		InputComponent->BindAction("RightClick", IE_Released, this, &ADragonController::InputMouseRightReleased);
		InputComponent->BindAction("KeyboardSpace", IE_Released, this, &ADragonController::InputMouseSpacePressed);
		InputComponent->BindAction("KeyboardQ", IE_Released, this, &ADragonController::InputQPressed);
		InputComponent->BindAction("KeyboardW", IE_Pressed, this, &ADragonController::InputWPressed);
		InputComponent->BindAction("KeyboardW", IE_Released, this, &ADragonController::InputWUnPressed);
		InputComponent->BindAction("KeyboardC", IE_Released, this, &ADragonController::InputCPressed);
	}
}

void ADragonController::MoveToMouseCursor()
{
	GetHitResultUnderCursor(ECC_Visibility, false, mouseHit);
	if (mouseHit.bBlockingHit)
	{
		SetNewDestination(mouseHit.ImpactPoint);
		if (dragonCharacter)
		{
			FVector mouseToTarget = mouseHit.ImpactPoint - dragonCharacter->GetActorLocation();
			FRotator mouseLookDirection = mouseToTarget.Rotation();
			mouseLookDirection.Pitch = 0.f;
			mouseLookDirection.Roll = 0.f;
			
			FRotator currentRot = GetControlRotation();
			FRotator newRot = FMath::RInterpTo(currentRot, mouseLookDirection, GetWorld()->GetDeltaSeconds(), 20.f);
			if (dragonCharacter->bFlying)
			{
				FVector target = mouseHit.ImpactPoint;
				target.Z = dragonCharacter->GetActorLocation().Z; 
				dragonCharacter->MoveToInAir(target);

				SetControlRotation(newRot);
			}
			else
			{
				SetControlRotation(newRot);
			}
		}
	}
}

void ADragonController::CheckMouseCursorShape()
{
	FHitResult mouseHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, mouseHitResult);
	if (mouseHitResult.bBlockingHit)
	{
		AActor* hitActor = mouseHitResult.GetActor();
		if (hitActor && hitActor->GetName().Contains(TEXT("Crow")) || hitActor && hitActor->GetName().Contains(TEXT("Skeleton")))
		{
			if (!bOnMouseCursorRecord)
			{
				bOnMouseCursorRecord = true;
				CurrentMouseCursor = EMouseCursor::Crosshairs;
			}
			return;
		}

		if (bOnMouseCursorRecord)
		{
			bOnMouseCursorRecord = false;
			CurrentMouseCursor = EMouseCursor::Default;
		}
	}
}

void ADragonController::SetNewDestination(const FVector destination)
{
	if (dragonCharacter)
	{
		float const distance = FVector::Dist(destination, dragonCharacter->GetActorLocation());
		if (distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, destination);
		}
	}	
}



void ADragonController::InputMouseRightPressed()
{
	bclickRightMouse = true;
}

void ADragonController::InputMouseRightReleased()
{
	bclickRightMouse = false;

	
}

void ADragonController::InputMouseSpacePressed()
{
	if (dragonCharacter && !bSpaceToggleCooldown)
	{
		bSpaceToggleCooldown = true;
		dragonCharacter->ToggleFlying();
		dragonCharacter->dragonWidget->updateSpaceCountDown(5.0f);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				FTimerHandle th_SpaceHandle;
				GetWorld()->GetTimerManager().SetTimer(th_SpaceHandle, FTimerDelegate::CreateLambda([this]()
					{
						bSpaceToggleCooldown = false;
					}), 5.0f, false);
			});
		
	}
}

void ADragonController::InputQPressed()
{
	if (dragonCharacter->bFlying) return;
	if (dragonCharacter && !bQToggleCooldown)
	{
		bQToggleCooldown = true;
		dragonCharacter->DragonQAttack();
		dragonCharacter->dragonWidget->updateQCountDown(1.5f);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				FTimerHandle th_QHandle;
				GetWorld()->GetTimerManager().SetTimer(th_QHandle, FTimerDelegate::CreateLambda([this]()
					{
						bQToggleCooldown = false;
					}), 1.0f, false);
			});
	}
}

void ADragonController::InputWPressed()
{
	if (bWToggleCooldown) return;
	
	if (dragonCharacter)
	{
		if (!dragonCharacter->bFlying)
		{
			dragonInstance->bGliding = false;
			dragonCharacter->UpdateLookAngleToMouse();
			dragonCharacter->DragonWAttack();
			if (dragonCharacter->dragonOrb)
			{
				dragonCharacter->dragonOrb->bFire = false;
			}
		}
		else
		{
			dragonInstance->bGliding = true;
			dragonCharacter->UpdateLookAngleToMouse();
			dragonCharacter->DragonWAttack();
			if (dragonCharacter->dragonOrb)
			{
				dragonCharacter->dragonOrb->bFire = false;
			}
		}
		
		
	}
}

void ADragonController::InputWUnPressed()
{
	if (bWToggleCooldown) return;
	if (dragonCharacter && !bWToggleCooldown)
	{
		bWToggleCooldown = true;
		dragonCharacter->UpdateCancleLookAngleToMouse();
		dragonCharacter->dragonWidget->updateWCountDown(1.5f);
		if (dragonCharacter->dragonOrb)
		{
			dragonCharacter->dragonOrb->bFire = true;
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				FTimerHandle th_WHandle;
				GetWorld()->GetTimerManager().SetTimer(th_WHandle, FTimerDelegate::CreateLambda([this]()
					{
						bWToggleCooldown = false;
					}), 1.5f, false);
			});
	}
}

void ADragonController::InputCPressed()
{
	dragonCharacter->PolyMolfCancle();

}
