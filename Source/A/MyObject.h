// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 * 
 */

UCLASS()
class A_API UMyObject : public UClass
{
    GENERATED_BODY()
public:
    UPROPERTY(SaveGame)
    float test;
    
    virtual void Serialize(FArchive& Ar) override;
};

