// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldGunInterface.h"

#include "Tower.h"

// Add default functionality here for any IHoldGunInterface functions that are not pure virtual.
AGunBase* IHoldGunInterface::GetWeapon()
{
	auto owner = Cast<AActor>(this);
	if (!owner) { return nullptr; }
	AGunBase* gun = nullptr;
	owner->ForEachAttachedActors(
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

void IHoldGunInterface::EquipWeapon(AGunBase* Gun)
{
	auto owner = Cast<AActor>(this);

	if (Gun)
		Gun->AttachToActor(owner, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		                                                    EAttachmentRule::SnapToTarget,
		                                                    EAttachmentRule::KeepWorld, false),
		                   FName("Weapon"));
	auto gun = GetWeapon();
	if (gun)
	{
		//装备武器成功
		if (owner->GetClass()->IsChildOf(ATower::StaticClass()))
		{
			//如果是装到了tower上激活controller
			auto tower = Cast<ATower>(owner);
			tower->AutoInitController();
		}
		UE_LOG(LogTemp, Warning, TEXT("挂载gun成功"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun挂载失败 %d"), (gun));
	}
}
