// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingActor.h"
#include "Math/UnrealMathUtility.h"
#include<algorithm>
#include "Global.h"
#include<string>



#include "Tower.h"
#include "Engine/EngineTypes.h"
using std::to_string;
// Sets default values
ABuildingActor::ABuildingActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("building"));
    SuperMesh->SetWorldScale3D(FVector(1, 1, 1));
    SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    this->SetRootComponent(SuperMesh);
    BuildingList.Add(ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/BTower")).Class);
    BuildingList.Add(ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/BTower1")).Class);
}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay()
{
    Super::BeginPlay();
    NextBuilding();
}

void ABuildingActor::init(AACharacter* characteri)
{
    UE_LOG(LogTemp, Warning, TEXT("building Actor init "));

    character = characteri;
    character->PlayerInputComponent
             ->BindAction("NextBuilding", IE_Pressed, this, &ABuildingActor::NextBuilding);
    character->PlayerInputComponent
             ->BindAction("PutBuilding", IE_Pressed, this, &ABuildingActor::PutBuilding);
}

float NearestModNumber(float num, float mod)
{
    float minn = FMath::FloorToFloat(num / mod) * mod;
    float maxn = minn + mod;
    return num - minn < maxn - num ? minn : maxn;
}

FVector GridWorld(FVector pos, float mod)
{
    const FVector output(pos.X,
                         NearestModNumber(pos.Y, mod),
                         NearestModNumber(pos.Z, mod));
    return output;
}

void ABuildingActor::NextBuilding()
{
    BuildingId++;
    BuildingId %= BuildingList.Num();
    FVector Location = this->GetTransform().GetLocation();

    if(ActorToShow)
    {
        ActorToShow->Destroy();
    }
    // TArray<AActor*> ChildrenA;
    // this->GetAttachedActors(ChildrenA);
    // for (auto Child : ChildrenA)
    // {
    //     Child->Destroy();
    // }
    AActor* NewActor = GetWorld()->SpawnActor<AActor>(BuildingList[BuildingId], Location, FRotator(0), FActorSpawnParameters());
    //下一行取消注释会报错
    // NewActor->SetReplicates(true);
    //附加到当前的建筑游标上
    NewActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
                                                            EAttachmentRule::KeepWorld,
                                                            EAttachmentRule::KeepWorld, false),
                            FName("building"));

    ActorToShow=NewActor;
    UE_LOG(LogTemp, Warning, TEXT("QQ"));
}


/**
 * 放置建筑
 */
void ABuildingActor::PutBuilding()
{
    const FVector Location = this->GetTransform().GetLocation();
    if (UGlobal::GetInstance()->TowerArray.Contains(Location))
    {
        UE_LOG(LogTemp, Error, TEXT("cant build at same position"));
        return;
    }

    //先创建tower的数据
    const auto TowerDataCore = new FTowerDataCore();
    //设置class 和position
    TowerDataCore->TowerClass = BuildingList[BuildingId];
    TowerDataCore->Location=Location;
    //传入world 创建actor
    const auto Tower=TowerDataCore->GenerateTower(GetWorld());
    

    UGlobal::GetInstance()->TowerArray.Add(Location, Tower);
}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //取出当前玩家的PlayerController
    if (character == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("building Actor character no found"));
        return;
    }
    const auto PlayerController = Cast<APlayerController>(character->Controller);
    const auto CharacterTransform = character->GetTransform();
    FVector Start, Dir, End;
    //根据鼠标位置 计算出朝向和视角起点的世界坐标
    PlayerController->DeprojectMousePositionToWorld(Start, Dir); //获取初始位置和方向

    if (Start.X + Start.Y + Start.Z == 0)
    {
        //鼠标移出屏幕
    }
    else
    {
        //矢量求终点
        End = Start + (Dir * 8000.0f); //设置追踪终点
        // GetWorld()->LineTraceSingleByChannel(HitResult, Start.Location, End.Location, ECC_Visibility);
        //两点和平面相交 找交点
        FVector pos = FMath::LinePlaneIntersection(Start, End,
                                                   FVector(CharacterTransform.GetLocation().X, 0, 0),
                                                   FVector(100, 0, 0));
        //按照100坐标对世界格子取整
        pos = GridWorld(pos, 100);
        // UE_LOG(LogTemp, Warning, TEXT("%s"),
        //        *pos.ToString());
        //把物体移动过去
        GetRootComponent()->SetWorldLocation(pos);
    }
}
