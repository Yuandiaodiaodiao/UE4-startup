// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"


UENUM(BlueprintType)
enum class EGunType:uint8
{
    None UMETA(DisplayName="无装备"),
    Pistol UMETA(DisplayName="手枪"),
    Smg UMETA(DisplayName="微型冲锋枪"),
    Shotgun UMETA(DisplayName="霰弹枪"),
    Rifle UMETA(DisplayName="步枪"),
    Launcher UMETA(DisplayName="发射器"),
    Special UMETA(DisplayName="特殊装备 "),
};

UCLASS(BlueprintType)
class A_API AGunBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGunBase();

    //用来决定枪支类型 蓝图可见
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    EGunType GunType;

    //射击的顶层实现
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "BlueprintFunc")
    void shoot(FVector StartLocation,FVector EndLocation);
    
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "BlueprintFunc")
    void shoot2();
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    
};
