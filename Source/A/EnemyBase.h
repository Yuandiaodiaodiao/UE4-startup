#include "GenericTeamAgentInterface.h"
#include "HealthDataInterface.h"
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyBase.generated.h"

UCLASS()
class A_API AEnemyBase : public APawn, public IGenericTeamAgentInterface, public IHealthDataInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	FHealthDataStruct HealthDataStruct;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//阵营id 2
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(2); }
};
