// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ACharacter.h"
#include "GameFramework/Actor.h"
#include "BuildingActor.generated.h"

UCLASS()
class A_API ABuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACharacter* character;
	
	ABuildingActor();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SuperMesh;
	int BuildingId=0;
	TArray<UClass*>BuildingList;
	void init(AACharacter* characteri);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void NextBuilding();
	
	void PutBuilding();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
