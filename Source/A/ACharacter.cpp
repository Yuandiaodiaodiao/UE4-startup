// Copyright Epic Games, Inc. All Rights Reserved.

#include "ACharacter.h"


#include "A.h"
#include "AISystem.h"
#include "BuildingActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VisualLogger/VisualLogger.h"
#include "K2Node_GetDataTableRow.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "MySaveGame.h"
#include "MyUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Perception/AIPerceptionSystem.h"


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
	InventoryClass = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/UI/TowerWidget_Yuan")).Class;
	// OnTakePointDamage.AddDynamic(this, &IHealthDataInterface::TakePointDamage);
	OnTakeAnyDamage.AddDynamic(this, &IHealthDataInterface::TakeAnyDamage);
	// OnTakeRadialDamage.AddDynamic(this, &IHealthDataInterface::TakeRadialDamage);


	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->bUsePawnControlRotation=true;
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);
	// 仅自身可见此网格体
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
}

void AACharacter::ShowInventory()
{
	auto State = (AMyPlayerState*)this->GetPlayerState();

	auto InventoryWidget = (UMyUserWidget*)CreateWidget(Cast<APlayerController>(this->GetController()), InventoryClass);
	InventoryWidget->InjectInventoryData(State->Inventory);
	InventoryWidget->AddToViewport();
	// auto data=State->Inventory.Top();
	auto datatable = LoadObject<UDataTable>(GetWorld(),TEXT("DataTable'/Game/GameData/TowerData.TowerData'"));
	auto rowa = datatable->FindRow<FTowerDataCore>(FName("a"), FString());
}

AGunBase* AACharacter::GetEquippedGun_Implementation()
{
	return nullptr;
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
	PlayerInputComponent->BindAction("TowerEquip", IE_Pressed, this, &AACharacter::TowerEquip);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AACharacter::ShowInventory);
	PlayerInputComponent->BindAction("PickUpTower", IE_Pressed, this, &AACharacter::PickUpTower);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AACharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AACharacter::TouchStopped);
}

void AACharacter::SetEquippedGun_Implementation(AGunBase* Gun)
{
}

void AACharacter::PickUpTower()
{
	auto tower = (ATower*)GetMouseSelected(ECC_Tower);
	if (tower == nullptr) return;

	auto key = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->
	           TowerArray.FindKey(tower);
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->TowerArray.Remove(*key);
	//加入
	auto State = (AMyPlayerState*)this->GetPlayerState();
	State->Inventory.Add(tower->GetData());
	UClass* tc = ATower::StaticClass();

	tower->Destroy();
}

void AACharacter::TowerEquip()
{
	//射线检测鼠标指向的tower
	auto tower = (ATower*)GetMouseSelected(ECC_Tower);
	if (tower == nullptr)return;
	UE_LOG(LogTemp, Warning, TEXT("selected tower"));

	//拿到手上装备着的gun
	auto gun = GetEquippedGun();
	auto ishavegun = gun == nullptr ? TEXT("get gun falied") : TEXT("get gun success");
	UE_LOG(LogTemp, Warning, TEXT("%s"), ishavegun);
	//拿tower的枪
	AGunBase* GunNow = tower->GetWeapon();
	//放枪 拔枪 互换
	if (!gun && GunNow)
	{
		//如果没有枪 尝试拔枪
		GunNow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule(), false));
	}
	if (!GunNow && gun)
	{
		//手上有枪 放枪

		gun->DetachFromActor(FDetachmentTransformRules(EDetachmentRule(), false));
	}
	if (GunNow && gun)
	{
		//互换
		GunNow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule(), false));
		gun->DetachFromActor(FDetachmentTransformRules(EDetachmentRule(), false));
	}
	SetEquippedGun(GunNow);
	tower->EquipWeapon(gun);
}

AActor* AACharacter::GetMouseSelected(ECollisionChannel TraceChannel)
{
	const auto PlayerController = Cast<APlayerController>(this->Controller);
	FVector Start, Dir, End;
	//根据鼠标位置 计算出朝向和视角起点的世界坐标
	auto bDirectionGet = PlayerController->DeprojectMousePositionToWorld(Start, Dir); //获取初始位置和方向

	//鼠标在屏幕内
	if (bDirectionGet)
	{
		//矢量求终点
		End = Start + (Dir * 8000.0f); //设置追踪终点
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TraceChannel);
		return HitResult.GetActor();
	}
	return nullptr;
}


FHealthDataStruct& AACharacter::GetHealthData()
{
	return HealthData;
}

void AACharacter::genBuildingActor()
{
	FVector Location = this->GetTransform().GetLocation();

	AActor* NewActor = GetWorld()->SpawnActor<ABuildingActor>(ABuildingActor::StaticClass(), Location, FRotator(0),
	                                                          FActorSpawnParameters());
	// NewActor->SetReplicates(true);

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

	TMap<FVector, ATower*> TowerArray;
	for (auto actor : Cast<AMyGameStateBase>(GetWorld()->GetGameState())->TowerArray)
	{
		auto tower = Cast<ATower>(actor.Value);
		if (auto towerfind = SaveGameInstance->TowerDataArray.Find(actor.Key))
		{
			//从读档的数据里load出当前位置的tower
			if (towerfind->Tower == tower)
			{
				//tower指针一样 说明旧的tower还没有被销毁 只替换tower数据
				tower->TowerData = towerfind;
				TowerArray.Add(actor.Key, actor.Value);
			}
			else
			{
				tower->Destroy();

				//创建新的tower 销毁旧的
				tower = towerfind->GenerateTower(GetWorld());
				TowerArray.Add(actor.Key, tower);
			}
		}
		else
		{
			//存档里没有 销毁炮塔
			tower->Destroy();
		}
	}
	for (auto towerData : SaveGameInstance->TowerDataArray)
	{
		if (TowerArray.Find(towerData.Key))
		{
			//这部分 已经处理过了
		}
		else
		{
			//新增炮塔
			auto tower = towerData.Value.GenerateTower(GetWorld());
			TowerArray.Add(towerData.Key, tower);
		}
	}
	//挂上新的TowerArray    
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->TowerArray = TowerArray;

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
		for (auto actor : Cast<AMyGameStateBase>(GetWorld()->GetGameState())->TowerArray)
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
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->TowerArray.Empty();
	UE_LOG(LogTemp, Warning, TEXT("Clean TowerArray"));
	genBuildingActor();
}
