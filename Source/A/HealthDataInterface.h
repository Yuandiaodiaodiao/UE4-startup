// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyMacro.h"
#include "UObject/Interface.h"
#include "HealthDataInterface.generated.h"

USTRUCT(BlueprintType)
struct FHealthDataStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
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


	virtual float GetHealth()
	{
		return GetHealthData().Health;
	}

	virtual FHealthDataStruct& GetHealthData() =0;

	UFUNCTION()
	virtual void TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	                     UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                     const UDamageType* DamageType, AActor* DamageCauser)
	{
		LOGWARNING("PointDamage from %s to %s value=%f", *DamageCauser->GetName(), *DamagedActor->GetName(),Damage)
		
	}

	UFUNCTION()
	virtual void TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                   class AController* InstigatedBy, AActor* DamageCauser)
	{
		LOGWARNING("AnyDamage from %s to %s value=%f", *DamageCauser->GetName(), *DamagedActor->GetName(),Damage)
		GetHealthData().Health -= Damage;
	}

	UFUNCTION()
	virtual void TakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	                      FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
	{
		LOGWARNING("RadialDamage from %s to %s value=%f", *DamageCauser->GetName(), *DamagedActor->GetName(),Damage)

	}
};
