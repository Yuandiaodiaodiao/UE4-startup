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
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

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
    cccc = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/Blueprints/BTower")).Class;
    bpo = ConstructorHelpers::FClassFinder<UObject>(TEXT("/Game/Blueprints/BPSave")).Class;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AACharacter::SetupPlayerInputComponent(class UInputComponent* InputComponentA)
{
    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

    PlayerInputComponent = InputComponentA;
    // set up gameplay key bindings
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAxis("MoveRight", this, &AACharacter::MoveRight);
    PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &AACharacter::SaveGame);
    PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &AACharacter::LoadGame);

    PlayerInputComponent->BindTouch(IE_Pressed, this, &AACharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AACharacter::TouchStopped);
    genBuildingActor();
}

void AACharacter::genBuildingActor()
{
    FVector Location = this->GetTransform().GetLocation();

    AActor* NewActor = GetWorld()->SpawnActor<AActor>(ABuildingActor::StaticClass(), Location, FRotator(), FActorSpawnParameters());
    NewActor->SetReplicates(true);

    if (PlayerInputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character begin4"));
        auto BuildingActor = Cast<ABuildingActor>(NewActor);
        BuildingActor->init(this);
        // PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    }
    UE_LOG(LogTemp, Log, TEXT("Character begin3"));
}

void AACharacter::LoadGame()
{
    UE_LOG(LogTemp, Warning, TEXT("in LoadGame"));
    UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AT"), 0));
   
    TMap<FVector,ATower*>TowerArray;
    for (auto actor : UGlobal::GetInstance()->TowerArray)
    {
        auto tower = Cast<ATower>(actor.Value);
        if (auto towerfind = SaveGameInstance->TowerDataArray.Find(actor.Key))
        {
            //从读档的数据里load出当前位置的tower
            if (towerfind->Tower == tower)
            {
                //tower指针一样 说明旧的tower还没有被销毁 只替换tower数据
                tower->TowerData = towerfind;
                TowerArray.Add(actor.Key,actor.Value);
            }
            else
            {
                tower->Destroy();

                //创建新的tower 销毁旧的
                tower=towerfind->GenerateTower(GetWorld());
                TowerArray.Add(actor.Key,tower);
            }
        }
        else
        {
            //存档里没有 销毁炮塔
            tower->Destroy();
            
        }
    }
    for(auto towerData:SaveGameInstance->TowerDataArray)
    {
        if(TowerArray.Find(towerData.Key))
        {
            //这部分 已经处理过了
        }else
        {
            //新增炮塔
            auto tower=towerData.Value.GenerateTower(GetWorld());
            TowerArray.Add(towerData.Key,tower);
        }
            
    }
    //挂上新的TowerArray    
    UGlobal::GetInstance()->TowerArray=TowerArray;

    UE_LOG(LogTemp, Warning, TEXT("%s"), *(SaveGameInstance->UserName));

    UE_LOG(LogTemp, Warning, TEXT("LoadGame finish"));
}

void AACharacter::SaveGame()
{
    UE_LOG(LogTemp, Warning, TEXT("in SaveGame2"));

    if (UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(
        UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())))
    {
        // 设置savegame对象上的数据。
        // SaveGameInstance->PlayerName = TEXT("PlayerOne");

        //保存所有的TowerData
        for (auto actor : UGlobal::GetInstance()->TowerArray)
        {
            auto atower = Cast<ATower>(actor.Value);
            SaveGameInstance->TowerDataArray.Add(actor.Key, *atower->TowerData);
        }


        //json反序列化
        // const FString InputString = TEXT("{\"grades\":\"test\"}");
        // TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( InputString );
        // TSharedPtr<FJsonObject> rRoot;
        // FJsonSerializer::Deserialize( Reader, rRoot );


        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("AT"),
                                             SaveGameInstance->UserIndex))
        {
            // 成功保存。
            UE_LOG(LogTemp, Warning, TEXT("SaveSuccess"));
        }
    }
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

    UGlobal::GetInstance()->TowerArray.Reset();
    UE_LOG(LogTemp, Warning, TEXT("Clean TowerArray"));
}

void AACharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    UE_LOG(LogTemp, Warning, TEXT("Character load in global a"));
    UGlobal::GetInstance()->character = this;
}
