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

UENUM(BlueprintType)
enum class FireMode:uint8
{
    Auto UMETA(DisplayName="全自动"),
    SemiAuto UMETA(DisplayName="半自动"),
    Safe UMETA(DisplayName="保险"),
};


USTRUCT(BlueprintType)
struct FShootDirectionStruct
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector StartLocation;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector EndLocation;
};
UCLASS(BlueprintType)
class A_API AGunBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGunBase();
    /////////////////////////////////////武器基础属性///////////////////////////////////////////////////
    //用来决定枪支类型 蓝图可见
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    EGunType GunType;
    //武器射程(对于即时命中武器是最大伤害距离，对于投射物是达到该距离触发伤害效果)
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float Range;
    //武器射速(次数/分钟)
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float RPM;
    //基础伤害
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float BasicDamage;
    //TODO 基础伤害类型  
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float BasicDamageType;
    //支持的开火模式
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TSet<FireMode> SupportedFireModes;
    //弹匣容量
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    int32 Magazine;
    //装弹时间
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float ReloadTime;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float TaticalReloadTime;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<UParticleSystem> GunFireEmitter;
    
    ///////////////////////////////////////////武器升级和自定义/////////////////////////////////////////
    //枪械模型
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<UStaticMeshComponent> Model;
    //对于直线型武器 返回起点终点 对于抛物线武器返回 矢量发射方向 和 矢量力
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="BlueprintHook")
    FShootDirectionStruct GetShootDirection();
    // 枪口粒子发射器
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<UParticleSystemComponent> MuzzleEmitter;
    // 备用-武器特效粒子
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<UParticleSystemComponent> WeaponVFSEmitter;
    // 承载武器改装的场景组件
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> Muzzle;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> Rail1;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> Rail2;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> Rail3;
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> Rail4;
    //  安装武器瞄具的场景组件
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    TObjectPtr<USceneComponent> RailSight;
    //TODO 各类BUFF的实现和结构，这里需要有一个针对改装后武器各种加成的集合，只在改装武器时更新，减少伤害计算时运算量。
    ////////////////////////////////武器事件//////////////////////////////////////////////////
    //射击的顶层实现
    UFUNCTION(BlueprintCallable, Category = "BlueprintFunc")
    void shoot();
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "BlueprintFunc")
    void shoot2();
    UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,  Category = "BlueprintFunc")
    void Reload(int32 Ammo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceiveDelegateEvent,bool,bSuccess,int32,Ammo);
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FReceiveDelegateEvent  ReloadEventDelegate;
    UFUNCTION(BlueprintCallable,Category = "BlueprintFunc")
    void ReloadEventDelegateBroadCast(bool bSuccess,int32 Ammo);
    ////////////////////////////////RunTime数据//////////////////////////////////////////////////
    //剩余弹药数量
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    int32 RoundRemain;
    //下次允许开火的时间
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    float AllowShootTime=0;
    //换弹阶段
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    int32 ReloadingStage;
    //是否被人、炮塔装备
    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    bool IsEquipped;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    
};
