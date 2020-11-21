// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryEntryWidget.h"


#include "MyUserWidget.h"
#include "Tower.h"
#include "Field/FieldSystemNodes.h"

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    auto Item = ((UFItemTableObject*)ListItemObject)->data;
    FString ss = Item->GenItemInfo();
    EditText->SetText(FText::FromString(ss));
}

void UInventoryEntryWidget::NativeConstruct()
{
    if (auto view = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("EditableTextBox_136")))))
    {
        EditText = view;
    }
}
