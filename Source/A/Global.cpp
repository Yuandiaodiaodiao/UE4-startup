// Fill out your copyright notice in the Description page of Project Settings.


#include "Global.h"


static UGlobal* instance;

UGlobal* UGlobal::GetInstance()
{
    return instance;
}

UGlobal::UGlobal()
{
    TowerArray.Reset();
   instance=this;
}

