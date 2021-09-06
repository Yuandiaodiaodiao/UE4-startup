// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"



//add viewport 创建页面
void UMyUserWidget::NativeConstruct()
{
    auto view = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView_30"))));
    if (view)
    {
        ListView = view;
    }
    if(ListView==NULL){return;}
    for(auto loop:Inventory)
    {
        auto z=NewObject<UFItemTableObject>(ListView);
        z->data=loop;
        ListView->AddItem(z);
    }
}


