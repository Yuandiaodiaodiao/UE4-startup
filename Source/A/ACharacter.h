// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "GenericTeamAgentInterface.h"
#include "GunBase.h"
#include "HealthDataInterface.h"
#include "GameFramework/Character.h"
#include "ACharacter.generated.h"


UCLASS(config=Game)
class AACharacter : public ACharacter, public IHealthDataInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	TSubclassOf<UUserWidget> InventoryClass;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void LoadGame();
	void SaveGame();
	// End of APawn interface


public:
	void test()
	{
		GetHealth();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHealthDataStruct HealthData;

	FHealthDataStruct& GetHealthData();
	void genBuildingActor();
	UInputComponent* PlayerInputComponent;
	void BeginPlay();
	AACharacter();
	void ShowInventory();
	UGameInstance* instance;
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//获取AACharacter子类拿的枪
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AGunBase* GetEquippedGun();

	//设置AACharacter子类拿的枪
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetEquippedGun(AGunBase* Gun);

	void PickUpTower();
	//将手上的装备 转移给tower
	void TowerEquip();

	//获取玩家鼠标选中的物体
	AActor* GetMouseSelected(ECollisionChannel TraceChannel);
	FGenericTeamId GetGenericTeamId() const override;
};
