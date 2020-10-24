// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ACharacter.h"
#include "UObject/NoExportTypes.h"
#include "Global.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class A_API UGlobal : public UObject
{
    GENERATED_BODY()
private:
    
public:
    UFUNCTION(BlueprintCallable)
    static UGlobal* GetInstance();
    AACharacter* character;
    
    UFUNCTION(BlueprintCallable)
    AACharacter* Character() const
    {
        return character;
    }

    
    
    UGlobal();

    
};
