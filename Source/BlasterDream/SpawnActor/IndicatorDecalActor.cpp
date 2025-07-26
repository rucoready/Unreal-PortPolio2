// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorDecalActor.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/World.h"

AIndicatorDecalActor::AIndicatorDecalActor()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Luco/IndicatorAoe/M_RIndicator.M_RIndicator"));

    if (MaterialFinder.Succeeded())
    {
        DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialFinder.Object, this);
        GetDecal()->SetDecalMaterial(DynamicMaterialInstance);
    }
}

void AIndicatorDecalActor::BeginPlay()
{
    Super::BeginPlay();

    //setMPCScalar1Value();
    forceSetBlue();

    cacheOriginalBlueColor();
    //setMPCScalar0Value();


}
//blue -> red
void AIndicatorDecalActor::setMPCScalar1Value()
{
    //if (!this)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("DontCallFunction"));
    //    return;
    //}

    //if (!GetWorld())
    //{
    //   UE_LOG(LogTemp, Error, TEXT("GetworldNull"));
    //    return;
    //}
    //FString mpcLoad = TEXT("/Game/Luco/IndicatorAoe/PC_WForceIndicator");
    //UMaterialParameterCollection* mpc = LoadObject<UMaterialParameterCollection>(nullptr, *mpcLoad);
    //UMaterialParameterCollectionInstance* mpc_Instance = GetWorld()->GetParameterCollectionInstance(mpc);

    //if (mpc_Instance)
    //{
    //    FName mpcZeroBlueVector = mpc->VectorParameters[0].ParameterName;  
    //    FName mpcOneRedVector = mpc->VectorParameters[1].ParameterName;   

        
    //    if (g_OriginalBlueColor == FLinearColor::Transparent)
    //    {
    //        mpc_Instance->GetVectorParameterValue(mpcZeroBlueVector, g_OriginalBlueColor);
    //    }

     
     //   FLinearColor changedColor;
    //    if (mpc_Instance->GetVectorParameterValue(mpcOneRedVector, changedColor))
    //    {
    //        mpc_Instance->SetVectorParameterValue(mpcZeroBlueVector, changedColor);
    //    }
   // }
}
//red -> blue
void AIndicatorDecalActor::setMPCScalar0Value()
{
    //if (!this)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("DontCallFunction"));
     //   return;
    //}

    //if (!GetWorld())
    //{
    //    UE_LOG(LogTemp, Error, TEXT("GetworldNull"));
    //    return;
    //}
    //FString mpcLoad = TEXT("/Game/Luco/IndicatorAoe/PC_WForceIndicator");
    //UMaterialParameterCollection* mpc = LoadObject<UMaterialParameterCollection>(nullptr, *mpcLoad);
   
    

    //UMaterialParameterCollectionInstance* mpc_Instance = World->GetParameterCollectionInstance(mpc);

    //if (mpc_Instance)
    //{
    //    FName mpcZeroBlueVector = mpc->VectorParameters[0].ParameterName;

    //    mpc_Instance->SetVectorParameterValue(mpcZeroBlueVector, g_OriginalBlueColor);
    //}
}

void AIndicatorDecalActor::cacheOriginalBlueColor()
{
    FString mpcLoad = TEXT("/Game/Luco/IndicatorAoe/PC_WForceIndicator");
    UMaterialParameterCollection* mpc = LoadObject<UMaterialParameterCollection>(nullptr, *mpcLoad);
    UMaterialParameterCollectionInstance* mpc_Instance = GetWorld()->GetParameterCollectionInstance(mpc);

    if (mpc_Instance)
    {
        FName mpcZeroBlueVector = mpc->VectorParameters[0].ParameterName;

  
        if (g_OriginalBlueColor == FLinearColor::Transparent)
        {
            g_OriginalBlueColor = FLinearColor(0.f, 0.3f, 1.f, 1.f); 
        }

        mpc_Instance->GetVectorParameterValue(mpcZeroBlueVector, g_OriginalBlueColor);
    }
}

void AIndicatorDecalActor::forceSetBlue()
{
    FString mpcLoad = TEXT("/Game/Luco/IndicatorAoe/PC_WForceIndicator");
    UMaterialParameterCollection* mpc = LoadObject<UMaterialParameterCollection>(nullptr, *mpcLoad);
    UMaterialParameterCollectionInstance* mpc_Instance = GetWorld()->GetParameterCollectionInstance(mpc);

    if (mpc_Instance)
    {
        FName mpcZeroBlueVector = mpc->VectorParameters[0].ParameterName;

        FLinearColor trueBlue = FLinearColor(0.f, 0.3f, 1.f, 1.f); 
        FLinearColor emissiveBlue = trueBlue * 20.0f;  

        mpc_Instance->SetVectorParameterValue(mpcZeroBlueVector, emissiveBlue);
    }
}
