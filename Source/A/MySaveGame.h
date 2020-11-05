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

    //全局的建筑物存档
    UPROPERTY(SaveGame)
    TMap<FVector,FTowerDataCore>TowerDataArray;
    
 
};
