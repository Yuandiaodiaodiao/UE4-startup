// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "TowerAIController.h"


FString FTowerDataCore::GenItemInfo()
{
	FString ss = this->Location.ToString();
	FString s2 = this->TowerClass->GetName();
	return ss + s2;
}

ATower* FTowerDataCore::GenerateTower(UWorld* world)
{
	ATower* NewActor = world->SpawnActor<ATower>(TowerClass, Location, FRotator(0), FActorSpawnParameters());
	// NewActor->SetReplicates(true);
	auto CRootComponent = Cast<UStaticMeshComponent>(NewActor->GetRootComponent());
	CRootComponent->SetCollisionProfileName(FName("Tower"));
	Tower = Cast<ATower>(NewActor);
	Tower->TowerData = this;
	auto profilename = CRootComponent->GetCollisionProfileName();

	return Tower;
}

// Sets default values
ATower::ATower()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("building"));
	//这话对子类好像没用
	SuperMesh->SetCollisionProfileName(FName("Tower"));
	//static之后其他new出来的实例大概不用重新绑?
	// SuperMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// this->SetActorEnableCollision(false);
	static auto mesh = ConstructorHelpers::FObjectFinderOptional<UStaticMesh>(
		TEXT("StaticMesh'/Game/Geometry/Meshes/Tower_Cube_Yuan.Tower_Cube_Yuan'")).Get();
	//设置模型

	SuperMesh->SetStaticMesh(mesh);
	SuperMesh->SetCollisionProfileName(FName("Tower"));

	this->SetRootComponent(SuperMesh);
	auto profilename = SuperMesh->GetCollisionProfileName();
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


	auto gun = GetWeapon();
	if (gun)
	{
		// UE_LOG(LogTemp, Warning, TEXT("射了！"));
		// gun->shoot2();
	}
}

void ATower::EquipWeapon(AGunBase* Gun)
{
	if (Gun)
		Gun->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		                                                   EAttachmentRule::SnapToTarget,
		                                                   EAttachmentRule::KeepWorld, false),
		                   FName("Weapon"));
	auto gun = GetWeapon();
	if (gun)
	{
		//装备武器成功
		if (this->TowerAIController == nullptr)
		{
			TowerAIController = GetWorld()->SpawnActor<ATowerAIController>(
				ATowerAIController::StaticClass(), FActorSpawnParameters());
			if (TowerAIController)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s towercontroller挂载成功 %s"), *this->GetName(),
				       *TowerAIController->GetName());
				TowerAIController->Possess(this);
			}
		}
		//检查并赋予controller
		UE_LOG(LogTemp, Warning, TEXT("挂载gun成功"));
		gun->shoot2();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), (gun));
	}
}

AGunBase* ATower::GetWeapon()
{
	AGunBase* gun = nullptr;
	this->ForEachAttachedActors(
		[&gun](AActor* Actor)
		{
			if (Actor->GetClass()->IsChildOf(AGunBase::StaticClass()))
			{
				gun = Cast<AGunBase>(Actor);
				return false;
			}
			else return true;
		});

	return gun;
}

FItemTable* ATower::GetData()
{
	return TowerData;
}

FGenericTeamId ATower::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}
