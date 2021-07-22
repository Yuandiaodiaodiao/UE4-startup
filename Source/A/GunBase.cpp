// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"


#include "CollisionDebugDrawingPublic.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGunBase::AGunBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Model"));
	MuzzleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleEmitter"));
	MuzzleEmitter->SetupAttachment(Model);
	WeaponVFSEmitter = CreateDefaultSubobject<UParticleSystemComponent> (TEXT("WeaponVFSEmitter"));
	WeaponVFSEmitter->SetupAttachment(Model);
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Model);
	Muzzle->SetRelativeLocation(FVector(0,0,0));
	Rail1 = CreateDefaultSubobject<USceneComponent>(TEXT("Rail1"));
	Rail1->SetupAttachment(Model);
	Rail2 = CreateDefaultSubobject<USceneComponent>(TEXT("Rail2"));
	Rail2->SetupAttachment(Model);
	Rail3 = CreateDefaultSubobject<USceneComponent>(TEXT("Rail3"));
	Rail3->SetupAttachment(Model);
	Rail4 = CreateDefaultSubobject<USceneComponent>(TEXT("Rail4"));
	Rail4->SetupAttachment(Model);
}

void AGunBase::shoot()
{
	if(GetWorld()->GetTimeSeconds()>=AllowShootTime)
	{
	UE_LOG(LogTemp,Display,TEXT("%d"),RoundRemain)
	if(RoundRemain>0)
	{
		if (true)//todo 根据武器发射物类型决定
		{
			TArray<struct FHitResult> OutHits;
			FVector StartPosition=Muzzle->GetComponentLocation();
			UE_LOG(LogTemp,Display,TEXT("SSS:%s"),*StartPosition.ToString())
			FVector EndPosition=Muzzle->GetComponentLocation()+Range*Muzzle->GetForwardVector();
			//UKismetSystemLibrary::LineTraceMulti(GetWorld(),Muzzle->GetComponentLocation(),Muzzle->GetComponentLocation()+Range*Muzzle->GetForwardVector(),ETraceTypeQuery::,0,NULL,EDrawDebugTrace::ForDuration,OutHits,1);
			GetWorld()->LineTraceMultiByChannel(OutHits,StartPosition,EndPosition,ECollisionChannel::ECC_Visibility);
            //MuzzleEmitter->Activate();
			//TODO 对射线检测结果进行处理
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),GunFireEmitter,StartPosition);
			RoundRemain--;
			UE_LOG(LogTemp,Display,TEXT("%d"),RoundRemain)
			AllowShootTime=GetWorld()->GetTimeSeconds()+60/RPM;
		}
	}
    else
    {
    	UE_LOG(LogTemp,Display,TEXT("No Ammo"))
    }
	}
	
}



void AGunBase::shoot2_Implementation()
{
}

FShootDirectionStruct AGunBase::GetShootDirection_Implementation()
{
	return FShootDirectionStruct();
}

void AGunBase::ReloadEventDelegateBroadCast(bool bSuccess,int32 Ammo)
{
	ReloadEventDelegate.Broadcast(bSuccess,Ammo);
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
