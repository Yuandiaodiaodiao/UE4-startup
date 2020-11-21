// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "UObject/ObjectMacros.h"

#include "InventoryEntryWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class A_API UInventoryEntryWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()
public:
    UEditableTextBox* EditText;

   virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
  virtual void NativeConstruct() override;  
};
