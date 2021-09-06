// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "UObject/ObjectMacros.h"


#include "MyUserWidget.generated.h"


UCLASS()
class A_API UFItemTableObject : public UObject
{
    GENERATED_BODY()
public:
    FItemTable* data;
};

/**
 * 
 */
UCLASS(BlueprintType)
class A_API UMyUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    UListView* ListView;

    TArray<FItemTable*> Inventory;

    void InjectInventoryData(TArray<FItemTable*>& InventoryIn)
    {
        Inventory = InventoryIn;
    }

    virtual void NativeConstruct() override;
};
