// Fill out your copyright notice in the Description page of Project Settings.

#include "Math/UnrealMathUtility.h"
#include "BuildingActor.h"
#include<algorithm>
#include "Global.h"
#include<string>
using std::to_string;
// Sets default values
ABuildingActor::ABuildingActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("building"));
    SuperMesh->SetWorldScale3D(FVector(1, 1, 1));
}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay()
{
    Super::BeginPlay();
}

float NearestModNumber(float num,float mod)
{
    float minn = FMath::FloorToFloat(num/mod)*mod; 
    float maxn = minn + mod;
    return num - minn < maxn -num?minn:maxn;
}

FVector GridWorld(FVector pos, float mod)
{
    const FVector output(pos.X,
        NearestModNumber(pos.Y,mod),
        NearestModNumber(pos.Z,mod));
    return output;
}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const auto Controller = UGlobal::GetInstance()->character->Controller;
    const auto PlayerController = Cast<APlayerController>(Controller);
    // FVector WorldLocation,WorldDirection;
    // PlayerController->DeprojectMousePositionToWorld(WorldLocation,WorldDirection);

    FHitResult HitResult, Start, Dir, End;
    PlayerController->DeprojectMousePositionToWorld(Start.Location, Dir.Location); //获取初始位置和方向

    End.Location = Start.Location + (Dir.Location * 8000.0f); //设置追踪终点
    GetWorld()->LineTraceSingleByChannel(HitResult, Start.Location, End.Location, ECC_Visibility);
    // FVector pos = HitResult.Location;//位置
    // // pos.X=std::max(1200.0f,pos.X);
    // pos.X=1200;
    //
    // UE_LOG(LogTemp, Warning, TEXT("%s"),
    //        *HitResult.Location.ToString());
    FVector pos = FMath::LinePlaneIntersection(Start.Location, End.Location,
                                               FVector(1200, 0, 0), FVector(100, 0, 0));

    
    pos=GridWorld(pos,100);
    UE_LOG(LogTemp, Warning, TEXT("%s"),
           *pos.ToString());
    GetRootComponent()->SetWorldLocation(pos);
}
