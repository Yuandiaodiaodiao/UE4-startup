// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "TowerAIController.generated.h"


class A_API ATower;
/**
 * 
 */
UCLASS()
class A_API ATowerAIController : public AAIController
{
	GENERATED_BODY()
	
	ATowerAIController();
	UAIPerceptionComponent* AiComponent;
	public:
	TArray<AActor*>ActorsBeSee;
	ATower* GetTower() ;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>&ActorArray);
};
