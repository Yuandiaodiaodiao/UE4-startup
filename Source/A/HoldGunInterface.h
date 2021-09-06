// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GunBase.h"
#include "UObject/Interface.h"
#include "HoldGunInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHoldGunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class A_API IHoldGunInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	AGunBase* GetWeapon();
	void EquipWeapon(AGunBase* Gun);
};
