// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

/**
 * Tower的基类 能被建造 破坏 有ai 能进行攻击
 *
 */
UCLASS()
class A_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SuperMesh;
	
	UPROPERTY(BlueprintReadWrite)
	UParticleSystemComponent* shoot;
	
};
