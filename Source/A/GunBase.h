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

    UPROPERTY(BlueprintReadWrite)
    EGunType GunType;


    UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Snowing|BlueprintFunc")
    void shoot(FVector StartLocation,FVector EndLocation);


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    
};
