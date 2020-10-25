// Copyright Epic Games, Inc. All Rights Reserved.

#include "ACharacter.h"

#include "BuildingActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VisualLogger/VisualLogger.h"
#include "Global.h"

AACharacter::AACharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create a camera boom attached to the root (capsule)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->TargetArmLength = 500.f;
    CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
    CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

    // Create a camera and attach to boom
    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->GravityScale = 2.f;
    GetCharacterMovement()->AirControl = 0.80f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.f;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
    GetCharacterMovement()->MaxFlySpeed = 600.f;
    UE_LOG(LogTemp, Log, TEXT("Character inited0"));
    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AACharacter::SetupPlayerInputComponent(class UInputComponent* InputComponentA)
{
    PlayerInputComponent = InputComponentA;
    // set up gameplay key bindings
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveRight", this, &AACharacter::MoveRight);

    PlayerInputComponent->BindTouch(IE_Pressed, this, &AACharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AACharacter::TouchStopped);
}

void AACharacter::MoveRight(float Value)
{
    // add movement in that direction
    if (Value != 0)
    {
        // UE_LOG(LogTemp,Log,TEXT("right1233a"));
    }
    AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AACharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    // jump on any touch
    Jump();
}

void AACharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    StopJumping();
}

void AACharacter::BeginPlay()
{
    Super::BeginPlay();
    UGlobal::GetInstance()->character = this;
    
    FVector Location = this->GetTransform().GetLocation();
    FRotator Rotation(0, 0, 0);
    FActorSpawnParameters SpawnInfo;
    for (auto Child : this->Children)
    {    
        Child->Destroy();
    }
    AActor* NewActor = GetWorld()->SpawnActor<AActor>(ABuildingActor::StaticClass(), Location, Rotation, SpawnInfo);
    // NewActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative,
    //                                                         EAttachmentRule::KeepWorld,
    //                                                         EAttachmentRule::KeepWorld, true),
    //                         FName("BuildingActor"));
    if(PlayerInputComponent)
    {
        UE_LOG(LogTemp, Log, TEXT("Character begin4"));
    
        // PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    }
    UE_LOG(LogTemp, Log, TEXT("Character begin3"));
}
