// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "MyObject.h"
#include "TowerController.h"
#include "GameFramework/SaveGame.h"
#include "Tower.h"
#include "MySaveGame.generated.h"




/**
 * 
 */
UCLASS()
class A_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()
public:

    UMySaveGame();
    UPROPERTY(SaveGame)
    FString UserName;

    UPROPERTY(SaveGame)
    FString SaveSlotName;

    UPROPERTY(SaveGame)
    int32 UserIndex;

    UPROPERTY(SaveGame)
    int32 intTest;

    UPROPERTY(SaveGame)
    UTowerController* utc;

    UPROPERTY(SaveGame)
    UClass* c;

    UPROPERTY(SaveGame)
    AActor* a;

    UPROPERTY(SaveGame)
    TArray<UTowerController*> t;

    UPROPERTY(SaveGame)
    FTowerDataCore StructTesT;

    UPROPERTY(SaveGame)
    TMap<FVector,FTowerDataCore>TowerDataArray;
    
    UPROPERTY(SaveGame)
    UMyObject* StructTesC;
    //
    // UPROPERTY(SaveGame)
    // TArray<FJsonObject> js;
    //

    UPROPERTY(SaveGame)
    UObject* BPObject;
};
