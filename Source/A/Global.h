// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ACharacter.h"
#include "Global.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class A_API UGlobal : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static UGlobal* GetInstance();
    
    UPROPERTY(BlueprintReadWrite)
    AACharacter* character;
    
    UGlobal();

    
};
