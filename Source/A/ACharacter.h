// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACharacter.generated.h"

UCLASS(config=Game)
class AACharacter : public ACharacter
{
    GENERATED_BODY()

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

protected:

    /** Called for side to side input */
    void MoveRight(float Val);

    /** Handle touch inputs. */
    void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

    /** Handle touch stop event. */
    void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
    void genBuildingActor();
    void LoadGame();
    void SaveGame();
    // End of APawn interface


public:
    UInputComponent* PlayerInputComponent;
    void BeginPlay();
    void PostInitializeComponents();
    AACharacter();
    UGameInstance* instance;
    /** Returns SideViewCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    UClass* cccc;
    UClass* bpo;
};
