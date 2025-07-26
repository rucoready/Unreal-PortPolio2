


#include "DragonWOrb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "../MainCharacter/DragonCharacter.h"


ADragonWOrb::ADragonWOrb()
{

	PrimaryActorTick.bCanEverTick = true;
	//Scene
	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	RootComponent = sceneComp;
	//Collision
	collisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComponent"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetupAttachment(RootComponent);
	//SkeletalMeshComp
	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("niagaraComp"));
	niagaraComp->SetupAttachment(RootComponent);
	niagaraComp->SetWorldScale3D(FVector(0.2f));

	//ProjectTile
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectTileMevement"));
	projectileMovement->InitialSpeed = 0.f;
	projectileMovement->MaxSpeed = 0.f;
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->ProjectileGravityScale = 0.0f;
	bFire = false;
}

// Called when the game starts or when spawned
void ADragonWOrb::BeginPlay()
{
	Super::BeginPlay();

    collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADragonWOrb::OnBeginOverlapCollision);
	
}

void ADragonWOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!bFire)
    {
        if (AActor* ownerActor = GetOwner())
        {
            USkeletalMeshComponent* ownerMesh = ownerActor->FindComponentByClass<USkeletalMeshComponent>();
            if (ownerMesh)
            {
                FVector fireSocketLoc = ownerMesh->GetSocketLocation(TEXT("FireSocket"));
                FRotator fireSocketRot = ownerMesh->GetSocketRotation(TEXT("FireSocket"));

                SetActorLocationAndRotation(fireSocketLoc, fireSocketRot);
            }
        }

        if (currentScale < targetScale)
        {
            elaspedTime += DeltaTime;
            float growOrb = FMath::Clamp(elaspedTime / grownDuration, 0.f, 1.f);
            currentScale = FMath::Lerp(1.f, targetScale, growOrb);
            FVector newScale(currentScale);
            sceneComp->SetWorldScale3D(newScale);
			
        }
		else
		{
			if (!bOnceChargeSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(this, chargeEndSound, GetActorLocation());
				bOnceChargeSound = true;
			}
		}
    }
    else
    {
        projectileMovement->InitialSpeed = 3000.f;
        projectileMovement->MaxSpeed = 3000.f;
        FVector fireDir = GetActorForwardVector();
        projectileMovement->Velocity = fireDir * projectileMovement->InitialSpeed;
    }

    FRotator RotationDelta;
    RotationDelta.Pitch = 90.f * DeltaTime;
    RotationDelta.Yaw = 120.f * DeltaTime;
    RotationDelta.Roll = 60.f * DeltaTime;

    niagaraComp->AddLocalRotation(RotationDelta);
}

void ADragonWOrb::OnBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && bFire)
	{
		FString otherName = OtherActor->GetName();
		if (otherName.Contains(TEXT("Dragon")) || otherName.Contains(TEXT("SwordActor")) || otherName.Contains(TEXT("Crow")))
		{
			return;
		}
		FVector explosionLocation = GetActorLocation();
		AActor* spawnInstance = GetWorld()->SpawnActor<AActor>(explosionActor,explosionLocation,GetActorRotation());

		FVector effectSclae = FVector(10.f);
		FVector particleScale = FVector(1.3f);
		if (currentScale < targetScale)
		{
			effectSclae = FVector(10.0f * (1.0f / 3.0f));
			particleScale = FVector(1.5f * (1.0f / 3.0f));
		}
		if (spawnInstance)
		{
			spawnInstance->SetActorScale3D(effectSclae);
			spawnInstance->SetLifeSpan(0.2f);
		}
		UParticleSystemComponent* particleInstance = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),explosionParticle,explosionLocation,FRotator::ZeroRotator,true);
		if (particleInstance)
		{
			particleInstance->SetRelativeScale3D(particleScale);
		}

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			ADragonCharacter* dragonCharacter = Cast<ADragonCharacter>(playerController->GetCharacter());
			if (dragonCharacter)
			{
				UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
				dragonCharacter->WOrbHitCameraShake();
			}


		}
	
		Destroy();
	}

}

