// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values

//      void AEnemyBase::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
//                          UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
//                          const UDamageType* DamageType, AActor* DamageCauser)
// {
// 	LOGWARNING("PointDamage from %s to %s", *DamageCauser->GetName(), *DamagedActor->GetName())
//     
// }

AEnemyBase::AEnemyBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// AIControllerClass=
	AutoPossessAI = EAutoPossessAI::Disabled;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	static auto skele = ConstructorHelpers::FObjectFinderOptional<USkeletalMesh>(
		TEXT("SkeletalMesh'/Game/Enemy/Enemy2/柱体.柱体'")).Get();
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skele"));
	Skeletal->SetSkeletalMesh(skele);
	this->SetRootComponent(Skeletal);

	OnTakePointDamage.AddDynamic(this, &IHealthDataInterface::TakePointDamage);
	OnTakeAnyDamage.AddDynamic(this, &IHealthDataInterface::TakeAnyDamage);
	OnTakeRadialDamage.AddDynamic(this, &IHealthDataInterface::TakeRadialDamage);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}



// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
