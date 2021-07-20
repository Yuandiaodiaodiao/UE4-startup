// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthDataInterface.generated.h"

USTRUCT(BlueprintType)
struct FHealthDataStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	float MaxHealth;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class A_API IHealthDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	
	float GetHealth()
	{
		return GetHealthData().Health;
	}
	
	virtual FHealthDataStruct& GetHealthData();
	
};
