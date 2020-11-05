// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TowerController.generated.h"

/**
 * 
 */
UCLASS()
class A_API UTowerController : public UStruct
{
	GENERATED_BODY()
	UPROPERTY(SaveGame)
	int32 level=1;
	UTowerController();
	virtual void Serialize(FArchive& Ar) override;
	// virtual void Serialize(FStructuredArchive::FRecord Record) override;
	friend FArchive& operator<<(FArchive& Ar, UTowerController& SaveActorRef);
};
