// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnumLibrary.generated.h"
//AI当前状态，如巡逻，索敌，攻击
UENUM()
enum EAIBehaviour:uint8
{
	Idle   UMETA(DisplayName="空闲"),
	Patrol   UMETA(DisplayName="巡逻"),
	//警觉，但不会大幅移动搜索目标
	Vigilant   UMETA(DisplayName="警戒"),
	Searching   UMETA(DisplayName="搜寻"),
	Combat   UMETA(DisplayName="战斗"),
	Escaping   UMETA(DisplayName="脱离战斗中"),
	Controlled   UMETA(DisplayName="被控制"),
};



/**
 * 
 */
UCLASS()
class A_API Uenum : public UObject
{
	GENERATED_BODY()
};
