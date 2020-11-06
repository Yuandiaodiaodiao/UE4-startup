// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"


ATower* FTowerDataCore::GenerateTower(UWorld* world)
{
    AActor* NewActor = world->SpawnActor<AActor>(TowerClass, Location, FRotator(0), FActorSpawnParameters());
    NewActor->SetReplicates(true);
    auto CRootComponent = Cast<UStaticMeshComponent>(NewActor->GetRootComponent());
    CRootComponent->SetCollisionProfileName(FName("BlockAll"));
    Tower = Cast<ATower>(NewActor);
    Tower->TowerData = this;
    return Tower;
}

// Sets default values
ATower::ATower()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("building"));
    //static之后其他new出来的实例大概不用重新绑?
    SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    this->SetActorEnableCollision(false);
    static auto mesh = ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(
        TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")).Get();
    //设置模型
    SuperMesh->SetStaticMesh(mesh);
    this->SetRootComponent(SuperMesh);
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //射个激光
}
