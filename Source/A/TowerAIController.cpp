// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerAIController.h"
#include "Tower.h"
#include "DrawDebugHelpers.h"
#include "MyMacro.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

ATowerAIController::ATowerAIController()
{
	bAttachToPawn = 1;
	PrimaryActorTick.bCanEverTick = 1;
	AiComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI"));
	UAISenseConfig_Sight* AiConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfig"));
	AiConfigSight->SightRadius = 500;
	AiConfigSight->LoseSightRadius = 600;
	AiConfigSight->PeripheralVisionAngleDegrees = 180;
	AiConfigSight->DetectionByAffiliation.bDetectEnemies = 1;
	AiConfigSight->DetectionByAffiliation.bDetectFriendlies = 0;
	AiConfigSight->DetectionByAffiliation.bDetectNeutrals = 0;
	AiConfigSight->GetDebugColor();
	AiComponent->ConfigureSense(*AiConfigSight);
	AiComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AiComponent->OnPerceptionUpdated.AddDynamic(this, &ATowerAIController::OnPerceptionUpdated);
}

ATower* ATowerAIController::GetTower()
{
	return Cast<ATower>(GetPawn());
}

void ATowerAIController::Tick(float DeltaSeconds)
{
	if(this->GetTower()->GetWeapon())
	{
		this->GetTower()->GetWeapon()->shoot();
	}
	if (ActorsBeSee.Num() > 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("有了"));
		
		auto target = ActorsBeSee.HeapTop();
		auto ShootDirection=this->GetTower()->GetWeapon()->GetShootDirection();
		//两点 相对于
		auto Direction=ShootDirection.EndLocation-ShootDirection.StartLocation;
		auto TargetDirection=target->GetActorLocation()-ShootDirection.StartLocation;
		//枪自身的旋转
		auto WeaponRotation= UKismetMathLibrary::Conv_VectorToRotator(Direction);
		auto dot=UKismetMathLibrary::Dot_VectorVector(Direction,TargetDirection);
		//枪到 目标
		auto rotate = UKismetMathLibrary::FindLookAtRotation(ShootDirection.StartLocation,
                                                             target->GetActorLocation());
		auto rotateAns = UKismetMathLibrary::RInterpTo(WeaponRotation, rotate,
                                                       DeltaSeconds, 10);
		//拿到了新的旋转
		this->GetTower()->GetWeapon()->SetActorRotation(rotateAns);
		// this->GetPawn()->SetActorRotation(rotateAns);
	}
	DrawDebugCircle(GetWorld(), this->GetTargetLocation(), 200, 200 * 0.5, FColor::Red,
	                false, -1, 0, 4, FVector(0, 1, 0), FVector(0, 0, 1), false);
}

void ATowerAIController::OnPerceptionUpdated(const TArray<AActor*>& ActorArray)
{
	LOGWARNING("OnPerceptionUpdated%d",ActorArray.Num())
	ActorsBeSee = ActorArray;
	for(auto actor : ActorArray)
	{
		
		LOGWARNING("%s",*actor->GetName())
		const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(actor);
		if(OtherTeamAgent)
		{
			LOGWARNING("I ID=%d",OtherTeamAgent->GetGenericTeamId().GetId())
		}else
		{
			LOGWARNING("无阵容")
		}
	
	}
}

FGenericTeamId ATowerAIController::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}
