// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTCharacter.h"
#include "Component/ACTCharacterMovementComponent.h"
#include "Component/ACTIKSystemComp.h"
#include <Components/InputComponent.h>
#include <GameFramework/Controller.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Engine/World.h>
#include "Weapons/WeaponBasic.h"
#include <GameFramework/PawnMovementComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimInstance.h>
#include "Animation/ACTAnimInstance.h"
#include "Interface/LedgeInterface.h"
#include <DrawDebugHelpers.h>
#include <Private/KismetTraceUtils.h>
#include <Components/PawnNoiseEmitterComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/ArrowComponent.h>
#include <TimerManager.h>
#include "Spline/ACTSplineTurn.h"
#include <Components/SplineComponent.h>
#include "UI/ACTHUD.h"
#include <Animation/AnimMontage.h>
#include <ConstructorHelpers.h>
#include "UI/Task/UserWidget_ShowTask.h"
#include "UI/UserWidget_MainUI.h"
#include <EnumAsByte.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/PlayerController.h>
#include "Monster/GemActor.h"
#include <Camera/CameraActor.h>
#include "UEProject05GameModeBase.h"
#include <Sound/AmbientSound.h>
#include "UnrealNetwork.h"


// Sets default values
AACTCharacter::AACTCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UACTCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> takeSwordClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Equip_Montage.A_SW_Equip_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> closeSwordClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Unequip_Montage.A_SW_Unequip_Montage'"));
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageForwardMontageClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Damage/A_SW_Damage_Backward_Montage.A_SW_Damage_Backward_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageBackMontageClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Damage/A_SW_Damage_Fast_Montage.A_SW_Damage_Fast_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageLeftMontageClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Damage/A_SW_Damage_Left_Montage.A_SW_Damage_Left_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageRightMontageClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Damage/A_SW_Damage_Right_Montage.A_SW_Damage_Right_Montage'"));
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageClass(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/HoldWeapon/Death_1_Montage.Death_1_Montage'"));
	
	static ConstructorHelpers::FObjectFinder<USoundBase> DamageSoundClass(TEXT("SoundWave'/Game/ACTGame/Audios/Attack/Attack_Hit.Attack_Hit'"));

	IKSystemComp = CreateDefaultSubobject<UACTIKSystemComp>(TEXT("IKSystemComp"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	SprintArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	playerNoise = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PlayerNoise"));
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));
	TurnSpline = nullptr;
	EquipSwordMontage = takeSwordClass.Object;
	UnequipSwordMontage = closeSwordClass.Object;
	DamageForwardMontage = DamageForwardMontageClass.Object;
	DamageBackMontage = DamageBackMontageClass.Object;
	DamageLeftMontage = DamageLeftMontageClass.Object;
	DamageRightMontage = DamageRightMontageClass.Object;
	DeathMontage = DeathMontageClass.Object;

	SprintArmComp->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SprintArmComp);
	LeftArrow->SetupAttachment(RootComponent);
	RightArrow->SetupAttachment(RootComponent);

	LeftArrow->SetRelativeLocation(FVector(45, -45, 60));
	RightArrow->SetRelativeLocation(FVector(45, 45, 60));

	PlayerState = EPlayerState::EPS_Normal;

	TurnSplineMaxTime = 0.66;
	TurnSplineDeltaTime = 0;
	
	PlayerMaxHP = 20;
	PlayerHP = 20;

	bSpring = false;
	bIronSights = false;
	bJump = false;
	bisClimbing = false;
	bIsHangingTurn = false;
	bReceiveTask = false;
	bMovePlayer = false;
}

// Called when the game starts or when spawned
void AACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//TakeSwordMontage = Cast<UAnimMontage>(LoadClass<UAnimMontage>(this, TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Equip_Montage.A_SW_Equip_Montage'")));
	//CloseSwordMontage = Cast<UAnimMontage>(LoadClass<UAnimMontage>(this, TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Unequip_Montage.A_SW_Unequip_Montage'")));
	//EquipSwordMontage = Cast<UAnimMontage>(LoadObject<UAnimMontage>(this, TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Equip_Montage.A_SW_Equip_Montage'")));
	//UnequipSwordMontage = Cast<UAnimMontage>(LoadObject<UAnimMontage>(this, TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Equip/A_SW_Unequip_Montage.A_SW_Unequip_Montage'")));



}

FVector AACTCharacter::GetPawnViewLocation() const
{
	if (MainCamera)
	{
		return MainCamera->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation();
}

// Called every frame
void AACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//int32 a = IsCrouch() ? 1 : 0;
	//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
	IronsightsMoveTick(DeltaTime);
	LedgeTick(DeltaTime);
	WallHeightTracer(DeltaTime);
	//JumpUpTracer();
	HangingTurnTick(DeltaTime);
	MovePlayerTick(DeltaTime);

	//FVector startPos = RightArrow->GetComponentTransform().GetLocation() + RightArrow->GetRightVector() * 10 ;
	//FVector endPos = RightArrow->GetComponentTransform().GetLocation() - RightArrow->GetRightVector() * 20 ;
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
}

// Called to bind functionality to input
void AACTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AACTCharacter::InputMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AACTCharacter::InputMoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnUp"), this, &AACTCharacter::InputTurnUp);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &AACTCharacter::InputTurnRight);

	PlayerInputComponent->BindAction(TEXT("DoJump"), IE_Pressed, this, &AACTCharacter::InputDoJump);
	PlayerInputComponent->BindAction(TEXT("DoJump"), IE_Released, this, &AACTCharacter::InputDoStopJump);

	PlayerInputComponent->BindAction(TEXT("DoSpring"), IE_Pressed, this, &AACTCharacter::InputDoSpring);
	PlayerInputComponent->BindAction(TEXT("DoSpring"), IE_Released, this, &AACTCharacter::InputDoUnSpring);

	PlayerInputComponent->BindAction(TEXT("DoCrouch"), IE_Pressed, this, &AACTCharacter::InputDoCrouch);
	PlayerInputComponent->BindAction(TEXT("DoCrouch"), IE_Released, this, &AACTCharacter::InputDoUnCrouch);

	PlayerInputComponent->BindAction(TEXT("PickUp"), IE_Pressed, this, &AACTCharacter::InputPickUp);

	PlayerInputComponent->BindAction(TEXT("ThrowAway"), IE_Pressed, this, &AACTCharacter::InputThrowAway);

	PlayerInputComponent->BindAction(TEXT("Key_1"), IE_Pressed, this, &AACTCharacter::InputKey_1);
	PlayerInputComponent->BindAction(TEXT("Key_2"), IE_Pressed, this, &AACTCharacter::InputKey_2);
	PlayerInputComponent->BindAction(TEXT("Key_3"), IE_Pressed, this, &AACTCharacter::InputKey_3);

	PlayerInputComponent->BindAction(TEXT("DoMouseRight"), IE_Pressed, this, &AACTCharacter::DoIronsight);
	PlayerInputComponent->BindAction(TEXT("DoMouseRight"), IE_Released, this, &AACTCharacter::DoIronsight);

	PlayerInputComponent->BindAction(TEXT("LookTask"), IE_Released, this, &AACTCharacter::InputDoViewTask);

	PlayerInputComponent->BindAction(TEXT("DoRoll"), IE_Pressed, this, &AACTCharacter::DoRoll);
	PlayerInputComponent->BindAction(TEXT("DoRoll"), IE_Released, this, &AACTCharacter::DoUnRoll);

	PlayerInputComponent->BindAction(TEXT("Key_Tab"), IE_Pressed, this, &AACTCharacter::DoShowTask);

	PlayerInputComponent->BindAction(TEXT("DoAttack"), IE_Pressed, this, &AACTCharacter::DoBeingAttack);
	PlayerInputComponent->BindAction(TEXT("DoAttack"),IE_Released,this,&AACTCharacter::DoStopAttack);

	PlayerInputComponent->BindAction(TEXT("Key_Escape"), IE_Pressed, this, &AACTCharacter::InputEscape);

}


void AACTCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /* = 0 */)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

float AACTCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (PlayerState==EPlayerState::EPS_Death)
	{
		return 0;
	}
	PlayerHP -= Damage;
	//UE_LOG(LogTemp, Log, TEXT("111"));
	if (PlayerHP<=0)
	{
		//PlayMontage_Player(DeathMontage);
		SetPlayerState(EPlayerState::EPS_Death);
		UACTAnimInstance* animIns = Cast<UACTAnimInstance>(GetMesh()->GetAnimInstance());
		if (animIns)
		{
			animIns->SetDeath(true);
		}
		SetHPWidget();
		if (IsHoldWeapon())
		{
			InputThrowAway();
		}
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			FInputModeUIOnly inputUI;
			pc->SetInputMode(inputUI);
			
		}

		UGameplayStatics::SetGlobalTimeDilation(this, 0.3f);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AACTCharacter::DeathCallBack, 1);
		
		AUEProject05GameModeBase* gm = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
		if (gm)
		{
			gm->SetBGOn_Off(false);
		}

		return 0;
	}

	//UE_LOG(LogTemp, Log, TEXT("  player takedamage"));
	FVector damageDir = FVector(DamageCauser->GetActorLocation().X - GetActorLocation().X, DamageCauser->GetActorLocation().Y - GetActorLocation().Y, 0);
	damageDir.Normalize();

	float dirValue = FVector::DotProduct(GetActorForwardVector(), damageDir);
	float dirRightValue = FVector::DotProduct(GetActorRightVector(), damageDir);

	//UE_LOG(LogTemp, Log, TEXT("    ==== %f"),dirValue);
	if (dirValue>0.75f)
	{
		PlayMontage_Player(DamageForwardMontage);
	}
	else if(dirRightValue<-0.75)//&&dirRightValue <= 0.75f
	{
		PlayMontage_Player(DamageRightMontage);

	}
	else if (dirRightValue>0.75f)
	{
		PlayMontage_Player(DamageLeftMontage);

	}
	else
	{
		PlayMontage_Player(DamageBackMontage);
	}
	UGameplayStatics::PlaySound2D(this, DamgeSound);

	SetHPWidget();

	return 0;
}

void AACTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AACTCharacter, HoldWeapon);
	DOREPLIFETIME(AACTCharacter, LeftWeapon);
	DOREPLIFETIME(AACTCharacter, RightWeapon);
	DOREPLIFETIME(AACTCharacter, RightSword);
	DOREPLIFETIME(AACTCharacter, ChangeWeapon);
}

bool AACTCharacter::IsSpring() const
{


	return bSpring&&!bIsCrouched;
}

void AACTCharacter::PlayerMakeNoise(float Loudness, float MaxRange)
{
	//MakeNoise(Loudness, this, GetActorLocation(), MaxRange);
	playerNoise->MakeNoise(this,Loudness,GetActorLocation());
}

//Movenment
void AACTCharacter::InputMoveForward(float DeltaTime)
{
	MoveForwardValue = DeltaTime;
	
	if (PlayerState==EPlayerState::EPS_DND)
	{
		return;
	}
	if (!bHanging)
	{
		FVector moveDir = FRotationMatrix(FRotator(0, GetController()->GetDesiredRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
		AddMovementInput(moveDir*DeltaTime);
		return;
	}
	if (DeltaTime<0)
	{
		ReleaseLedge();
	}
	
}

void AACTCharacter::InputMoveRight(float DeltaTime)
{
	MoveRightValue = DeltaTime;

	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (!bHanging)
	{
		FVector moveDir = FRotationMatrix(FRotator(0, GetController()->GetDesiredRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);
		AddMovementInput(moveDir*DeltaTime);
		return;
	}

	HangingMove();
}

void AACTCharacter::InputTurnUp(float DeltaTime)
{
	
	AddControllerPitchInput(DeltaTime);

}

void AACTCharacter::InputTurnRight(float DeltaTime)
{

	AddControllerYawInput(DeltaTime);
}

void AACTCharacter::InputDoJump()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (!bHanging)
	{
		if (!IsCrouch())
		{
			Jump();
			bJump = !bJump;
		}
		return;
	}
	
	if (!IsHoldWeapon()&&MoveRightValue==0)
	{
		JumpTracer();
	}
}

void AACTCharacter::InputDoStopJump()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}
	if (!bHanging)
	{
		StopJumping();
		bJump = !bJump;
		return;
	}
	JumpTracer();
}



void AACTCharacter::DoBeingAttack()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (!IsHoldWeapon())
	{
		return;
	}
	//UE_LOG(LogTemp, Log, TEXT("  asdf"));
	HoldWeapon->BeginAttack();
}

void AACTCharacter::DoStopAttack()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (!IsHoldWeapon())
	{
		return;
	}
	//UE_LOG(LogTemp, Log, TEXT("  asdf"));
	HoldWeapon->StopAttack();
}

void AACTCharacter::InputDoSpring()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}
	bSpring = true;
}

void AACTCharacter::InputDoUnSpring()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}
	bSpring = false;
}

void AACTCharacter::InputDoCrouch()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}
	if (IsHanging())
	{
		return;
	}
	if (CanCrouch())
	{
		Crouch();
	}
	//else
	//{
	//	UnCrouch();
	//}
}

void AACTCharacter::InputDoUnCrouch()
{
	UnCrouch();
}

void AACTCharacter::InputPickUp()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (Role<ROLE_Authority)
	{
		Server_PickUp();
		
	}


	TArray<FHitResult> hits;
	FCollisionShape shap;
	shap.ShapeType = ECollisionShape::Sphere;
	shap.Sphere.Radius = 300;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(hits, GetActorLocation(), GetActorLocation(), FQuat(), ECC_Visibility, shap, param);

	AWeaponBasic* findWeapon = nullptr;
	float weaponDirValue = -1;
	for (auto hit:hits)
	{
		AGemActor* gem = Cast<AGemActor>(hit.Actor);
		if (gem)
		{
			gem->Destroy();
			
			AUEProject05GameModeBase* gm = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
			if (gm)
			{
				gm->SetGem(nullptr);
			}
			DoShowTask();
			return;
		}

		AWeaponBasic* _weapon = Cast<AWeaponBasic>(hit.Actor);
		if (_weapon)
		{
			if (!_weapon->IsHasOwner())
			{
				FVector weaponDir = GetActorLocation() - _weapon->GetActorLocation();
				weaponDir.Normalize();
				float _weaponDirValue = FVector::DotProduct(weaponDir,GetActorForwardVector());
				if (_weaponDirValue>weaponDirValue)
				{
					findWeapon = _weapon;
				}
			}
		}
	}
	if (findWeapon)
	{
		DoHoldWeapon(findWeapon);
	}

}
void AACTCharacter::InputThrowAway()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	DoThrowWeapon(HoldWeapon);
}
//Weapon
void AACTCharacter::InputKey_1()
{
	//AUEProject05GameModeBase* gm = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
	//if (gm)
	//{
	//	gm->ResetLevel();
	//	return;
	//}
	//Reset();

	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}
	EventKeyTakeWeapon(LeftWeapon, TEXT("LeftWeapon"));
}
void AACTCharacter::InputKey_2()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	EventKeyTakeWeapon(RightWeapon, TEXT("RightWeapon"));
}

void AACTCharacter::InputKey_3()
{
	if (PlayerState == EPlayerState::EPS_DND)
	{
		return;
	}

	if (IsHoldWeapon())
	{
		if (IsHoldSword())
		{
			PlayMontage_Player(UnequipSwordMontage);
		}
		else
		{
			if (RightSword)
			{
				TakeWeapon(HoldWeapon);
				HoldWeapon = nullptr;
				PlayMontage_Player(EquipSwordMontage);
			}
		}
	}
	else
	{
		if (RightSword)
		{
			PlayMontage_Player(EquipSwordMontage);
		}
	}
}


void AACTCharacter::EventKeyTakeWeapon(AWeaponBasic* weapon, FName slotName)
{
	if (!weapon)
	{
		if (IsHoldWeapon())
		{
			if (IsHoldSword())
			{
				return;
			}
			HoldWeapon->SetWeaponAttach(slotName);
			if (slotName==TEXT("LeftWeapon"))
			{
				LeftWeapon = HoldWeapon;
			}
			else
			{
				RightWeapon = HoldWeapon;
			}
			weapon = HoldWeapon;
			HoldWeapon = nullptr;
			bUseControllerRotationYaw = false;
		}
		return;
	}
	AWeaponBasic* _weapon = weapon;
	weapon = nullptr;
	if (HoldWeapon)
	{
		if (IsHoldSword())
		{
			PlayMontage_Player(UnequipSwordMontage);
			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, this, &AACTCharacter::ChangeWeaponCallBack, ReleaseDNDTime);
			ChangeWeapon = weapon;
			return;
		}
		weapon = HoldWeapon;
		if (slotName == TEXT("LeftWeapon"))
		{
			LeftWeapon = HoldWeapon;
		}
		else
		{
			RightWeapon = HoldWeapon;
		}
		weapon->SetWeaponAttach(slotName);
	}
	else
	{
		weapon = nullptr;
		if (slotName == TEXT("LeftWeapon"))
		{
			LeftWeapon = nullptr;
		}
		else
		{
			RightWeapon = nullptr;
		}
	}

	HoldOnWeapon(_weapon);
}

void AACTCharacter::ChangeWeaponCallBack()
{
	//UE_LOG(LogTemp, Log, TEXT("   asdf"));
	if (!ChangeWeapon||IsHoldWeapon())
	{
		return;
	}
	HoldWeapon = ChangeWeapon;
	HoldWeapon->SetWeaponAttach(TEXT("HoldWeapon"));
	bUseControllerRotationYaw = true;
}


bool AACTCharacter::IsHoldSword() const
{
	if (!IsHoldWeapon())
	{
		return false;
	}

	return HoldWeapon->IsSword();//&&IsHoldWeapon()
}

void AACTCharacter::NotifyEquipSword()
{
	if (!RightSword)
	{
		return;
	}
	if (IsHoldWeapon())
	{
		DoThrowWeapon(HoldWeapon);
	}

	HoldWeapon = RightSword;
	RightSword = nullptr;
	HoldWeapon->SetWeaponAttach(TEXT("HoldSword"));

	//PlayerState = EPlayerState::EPS_Normal;
	bUseControllerRotationYaw = true;
}

void AACTCharacter::NotifyUnequipSword()
{
	if (!IsHoldWeapon())
	{
		return;
	}
	if (RightSword)
	{
		DoThrowWeapon(HoldWeapon);
	}

	RightSword = HoldWeapon;
	HoldWeapon = nullptr;
	RightSword->SetWeaponAttach(TEXT("RightSword"));

	//PlayerState = EPlayerState::EPS_Normal;
	bUseControllerRotationYaw = false;
	
}

void AACTCharacter::DoIronsight()
{
	if (!IsHoldWeapon())
	{
		return;
	}

	bIronSights = !bIronSights;
}
void AACTCharacter::TakeWeapon(AWeaponBasic* weapon)
{
	bool bLeft = true;
	if (!LeftWeapon)
	{
		LeftWeapon = weapon;
	}
	else if (!RightWeapon)
	{
		bLeft = false;
		RightWeapon = weapon;
	}
	else
	{
		if (IsHoldWeapon())
		{
			DoThrowWeapon(HoldWeapon);
		}
		return;
	}

	weapon->SetWeaponOwner(this);
	weapon->SetWeaponAttach(bLeft ? TEXT("LeftWeapon") : TEXT("RightWeapon"));
}
void AACTCharacter::DoHoldWeapon(AWeaponBasic* weapon)
{
	if (HoldWeapon)
	{
		DoThrowWeapon(HoldWeapon);
	}
	HoldWeapon = weapon;
	HoldWeapon->SetWeaponOwner(this);
	if (IsHoldSword())
	{
		HoldWeapon->SetWeaponAttach(TEXT("HoldSword"));
	}
	else
	{
		HoldWeapon->SetWeaponAttach(TEXT("HoldWeapon"));
	}
	bUseControllerRotationYaw = true;
}
void AACTCharacter::DoThrowWeapon(AWeaponBasic* weapon)
{
	if (!HoldWeapon)
	{
		return;
	}

	HoldWeapon->AddActorLocalOffset(GetActorForwardVector()*80);
	HoldWeapon->SetWeaponOwner(nullptr);
	HoldWeapon = nullptr;
	bUseControllerRotationYaw = false;
}
void AACTCharacter::HoldOnWeapon(AWeaponBasic* weapon)
{

	HoldWeapon = weapon;
	HoldWeapon->SetWeaponAttach(TEXT("HoldWeapon"));

	bUseControllerRotationYaw = true;
}
void AACTCharacter::IronsightsMoveTick(float DeltaTime)
{
	if (IsIronSights())
	{
		aimDistance = FMath::FInterpTo(aimDistance, 0, DeltaTime, 4);
	}
	else
	{
		aimDistance = FMath::FInterpTo(aimDistance, 23, DeltaTime, 4);
	}
}



//ReleaseDNDTimer
void AACTCharacter::ReleaseDNDTimer(float time)
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AACTCharacter::ReleaseDNDTimerCallBack,time);
}

void AACTCharacter::ReleaseDNDTimerCallBack()
{
	PlayerState = EPlayerState::EPS_Normal;
}



//Play Montage
void AACTCharacter::PlayMontage_Player(UAnimMontage* montage)
{
	UACTAnimInstance* animIns = Cast<UACTAnimInstance>(GetMesh()->GetAnimInstance());
	if (animIns&&EquipSwordMontage)
	{
		PlayerState = EPlayerState::EPS_DND;
		ReleaseDNDTime = animIns->Montage_Play(montage);
	}
	ReleaseDNDTimer(ReleaseDNDTime);
}

//Ledge

void AACTCharacter::LedgeTick(float DeltaTime)
{
	ForwardTracer(DeltaTime);

}

void AACTCharacter::ForwardTracer(float DeltaTime)
{
	if (bisClimbing||IsHoldWeapon())//||bIsHangingTurn
	{
		return;
	}
	FVector playerPos = GetActorLocation();
	FHitResult forwardHit;
	FCollisionShape forwardshap;
	forwardshap.ShapeType = ECollisionShape::Capsule;
	forwardshap.Capsule.Radius = 20;
	FCollisionQueryParams forwardparam;
	forwardparam.AddIgnoredActor(this);
	//前面墙壁
	//DrawDebugCapsuleTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), 20,300,EDrawDebugTrace::ForDuration, false, forwardHit, FLinearColor::Red, FLinearColor::Green,0);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 300, FColor::Red);
	//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
	//UE_LOG(LogTemp, Log, TEXT("  ForwardTracer"));
	if (GetWorld()->SweepSingleByChannel(forwardHit, playerPos+GetActorUpVector()*10,
		playerPos + GetActorUpVector() * 10+FVector(GetActorForwardVector().X * 100,GetActorForwardVector().Y * 100, GetActorForwardVector().Z), FQuat(),ECC_GameTraceChannel1,forwardshap, forwardparam))
	{

		//wallNormal = forwardHit.Normal;
		//wallNormal = FVector(forwardHit.Normal.X, forwardHit.Normal.Y, 0);
		//wallLocation = forwardHit.Location/*FVector(forwardHit.Location.X,forwardHit.Location.Y,playerPos.Z)*/;
		//检测墙壁高度
		//AddActorLocalOffset(wallNormal);
		bFreeHanging = false;
		//UE_LOG(LogTemp, Log, TEXT("000000000000"));
	}
	else
	{
		bFreeHanging = true;
		//UE_LOG(LogTemp, Log, TEXT("1111111111111"));
	}
	

}

void AACTCharacter::WallHeightTracer(float DeltaTime)
{
	if (IsHoldWeapon())//!GetMovementComponent()->IsFalling()||
	{
		return;
	}
	FVector playerPos = GetActorLocation();
	FCollisionShape forwardshap;
	forwardshap.ShapeType = ECollisionShape::Capsule;
	forwardshap.Capsule.Radius = 20;
	FCollisionQueryParams forwardparam;
	forwardparam.AddIgnoredActor(this);
	FHitResult heightHit;
	FVector startPos = FVector(playerPos.X, playerPos.Y, playerPos.Z + 200) + GetActorForwardVector() * 70;
	//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
	if (GetWorld()->SweepSingleByChannel(heightHit, startPos, FVector(startPos.X,startPos.Y,startPos.Z-140), FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
	{
		
		TArray<FHitResult> wallHits;
		if (GetWorld()->SweepMultiByChannel(wallHits, startPos, FVector(startPos.X, startPos.Y, startPos.Z - 140), FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
		{

			FHitResult findlastWall = wallHits.Last();
			FVector middlePos = GetMesh()->GetSocketLocation(TEXT("MiddleSlot"));
			wallHeightLocation = findlastWall.Location;
			float wallHeight = findlastWall.Location.Z - middlePos.Z;

			//CodeMove
			FVector playerPos = GetActorLocation();
			FCollisionShape forwardshap;
			forwardshap.ShapeType = ECollisionShape::Capsule;
			forwardshap.Capsule.Radius = 5;
			FCollisionQueryParams forwardparam;
			forwardparam.AddIgnoredActor(this);
			FHitResult freeHit;
			FVector startPos = FVector(playerPos.X - GetActorForwardVector().X * 70, playerPos.Y - GetActorForwardVector().Y * 70, wallHeightLocation.Z - 21);
			//DrawDebugLine(GetWorld(), startPos, wallHeightLocation + FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, -11), FColor::Red);
			//DrawDebugLine(GetWorld(), FVector(0,0,0), FVector(0, 0, 0), FColor::Red);
			if (GetWorld()->LineTraceSingleByChannel(freeHit, startPos, wallHeightLocation + FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, -21), ECC_GameTraceChannel1, forwardparam))
				//if (GetWorld()->SweepSingleByChannel(freeHit, startPos, wallHeightLocation+ FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, 0),
				//	FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
			{
				wallNormal = freeHit.Normal;
				wallLocation = freeHit.Location + wallNormal * 25;
				//UE_LOG(LogTemp, Log, TEXT("   FreeHangingTracer"));
			}
			else
			{
				return;
			}


			if (wallHeight >= -10 && wallHeight <= 70)
			{
				
				
				if (GetMovementComponent()->IsFalling())
				{
					GrabLedge();
				}
			}
			if (wallHeight >= -40 && wallHeight <= 50)
			{
				bSlideTrace = false;
			}
			else
			{
				bSlideTrace = true;
			}
		}

	}
}

void AACTCharacter::GrabLedge()
{
	if (bHanging||bisClimbing)
	{
		return;
	}
	//FreeHangingTracer
	//if (bFreeHanging)
	//{
	//	FVector playerPos = GetActorLocation();
	//	FCollisionShape forwardshap;
	//	forwardshap.ShapeType = ECollisionShape::Capsule;
	//	forwardshap.Capsule.Radius = 20;
	//	FCollisionQueryParams forwardparam;
	//	forwardparam.AddIgnoredActor(this);
	//	FHitResult freeHit;
	//	FVector startPos = FVector(playerPos.X- GetActorForwardVector().X*70,playerPos.Y- GetActorForwardVector().Y * 70,wallHeightLocation.Z-21);
	//	//DrawDebugLine(GetWorld(), startPos, wallHeightLocation + FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, -11), FColor::Red);
	//	if (GetWorld()->LineTraceSingleByChannel(freeHit, startPos, wallHeightLocation + FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, -21), ECC_GameTraceChannel1, forwardparam))
	//	//if (GetWorld()->SweepSingleByChannel(freeHit, startPos, wallHeightLocation+ FVector(GetActorForwardVector().X * 10, GetActorForwardVector().Y * 10, 0),
	//	//	FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
	//	{
	//		wallNormal = freeHit.Normal;
	//		wallLocation = freeHit.Location+ wallNormal*25;
	//		//UE_LOG(LogTemp, Log, TEXT("   FreeHangingTracer"));
	//	}
	//	else
	//	{
	//		return;
	//	}
	//}

	FHitResult debugHit;
	FVector hangPos = GetMesh()->GetSocketLocation(TEXT("RightHandSlot"));
	if (GetWorld()->LineTraceSingleByChannel(debugHit, hangPos, hangPos-GetActorForwardVector()*50, ECC_GameTraceChannel1))
	{
		return;
	}



	bHanging = true;

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();
	
	FVector playerPos = FVector(wallLocation.X, wallLocation.Y,wallHeightLocation.Z-75)+wallNormal*15;


	//GetCapsuleComponent()->MoveComponent(playerPos,FRotator(0,0,0) , false);//FRotationMatrix(wallNormal.Rotation()) + FRotationMatrix(FRotator(0, -180, 0))).Rotator()
	//SetActorLocation(playerPos);
	MovePlayer(true, playerPos);
	//SetActorRotation((FRotationMatrix(wallNormal.Rotation()) + FRotationMatrix(FRotator(0, 0, 0))).Rotator());
	//SetActorRotation((FRotationMatrix(wallNormal.Rotation()) + FRotationMatrix(FRotator(0, 180, 0))).Rotator());
	SetActorRotation((wallNormal*-1).Rotation());
	//GetCapsuleComponent()->SetWorldRotation(wallNormal.Rotation());
	wallRotation = wallNormal.Rotation();
	
	ILedgeInterface* animInstance = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
	if (animInstance)
	{
		animInstance->Execute_CanGrab(GetMesh()->GetAnimInstance(), bHanging);
	}
}

void AACTCharacter::ReleaseLedge()
{
	bHanging = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	ILedgeInterface* animInstance = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
	if (animInstance)
	{
		animInstance->Execute_CanGrab(GetMesh()->GetAnimInstance(), bHanging);
	}
	if (bisClimbing)
	{
		return;
	}
	SetActorRotation(wallNormal.Rotation());
}

void AACTCharacter::JumpTracer()
{
	JumpUpTracer();
	if (bCanJumpUp)
	{
		NotifyJumpUp();
	}
	else
	{
		NotifyClimbUp();
	}
}

//JumpUp
void AACTCharacter::JumpUpTracer()
{
	if (bisClimbing||GetMovementComponent()->IsFalling())
	{
		return;
	}
	FVector playerPos = GetActorLocation();
	FCollisionShape forwardshap;
	forwardshap.ShapeType = ECollisionShape::Capsule;
	forwardshap.Capsule.Radius = 10;
	FCollisionQueryParams forwardparam;
	forwardparam.AddIgnoredActor(this);
	//forwardparam.bReturnPhysicalMaterial = true;
	FHitResult heightHit;
	//UE_LOG(LogTemp, Log, TEXT("  JumpUpTracer"));
	//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
	FVector startPos=FVector(wallLocation.X+GetActorForwardVector().X*30,wallLocation.Y+ GetActorForwardVector().Y * 30,playerPos.Z+500);
	FVector endPos = FVector(wallLocation.X + GetActorForwardVector().X * 30, wallLocation.Y + GetActorForwardVector().Y * 30, playerPos.Z + 100);
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
	if (GetWorld()->SweepSingleByChannel(heightHit, startPos, endPos, FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
	{
		//UE_LOG(LogTemp, Log, TEXT("  JumpUpTracer"));
		TArray<FHitResult> hits;
		//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
		//DrawDebugLine(GetWorld(),startPos, endPos, FColor::Red);
		if (GetWorld()->SweepMultiByChannel(hits, startPos, endPos, FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
		{
			FHitResult findhit = hits.Last();
			//FHitResult findhit = hits.Top();
			//FHitResult findhit = hits[hits.Num()-1];
			//FHitResult findhit = hits[0];
			
			//GetWorld()->LineTraceMultiByChannel()

			JumpUpTestVec = findhit.Location;
			//UE_LOG(LogTemp, Log, TEXT("   JumpUpTracer"));
			UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, JumpUpLaunchVec, GetActorLocation(), findhit.Location-GetActorForwardVector()*40);
			bCanJumpUp = true;
		}
	}
	else
	{
		bCanJumpUp = false;
	}
}

void AACTCharacter::NotifyJumpUp()
{
	bisClimbing = true;
	ILedgeInterface* animIns = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
	if (animIns)
	{
		animIns->Execute_JumpUp(GetMesh()->GetAnimInstance(), true);
	}
}
void AACTCharacter::JumpUp_Implementation(bool bClimbUp)
{
	bisClimbing = bClimbUp;
}

void AACTCharacter::SetHPWidget()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (!pc)
	{
		return;
	}
	AACTHUD* hud = Cast<AACTHUD>(pc->GetHUD());
	if (!hud)
	{
		return;
	}
	if (hud->GetMainUi())
	{
		hud->GetMainUi()->SetProgressBarPercent(PlayerHP / PlayerMaxHP);
	}
}

void AACTCharacter::ShowDeathUICallBack()
{
	APlayerController* pc = Cast <APlayerController>(GetController());
	if (!pc)
	{
		return;
	}

	AACTHUD* hud = Cast<AACTHUD>(pc->GetHUD());
	if (!hud)
	{
		return;
	}
	hud->ShowDeadUI();
}

void AACTCharacter::DeathCallBack()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		FVector cameraPos = GetActorLocation() + GetActorUpVector() * 500 - GetActorForwardVector() * 350;
		FVector cameraDir = GetActorLocation() - cameraPos;
		cameraDir.Normalize();
		FActorSpawnParameters spawnParam;
		//spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACameraActor* deathCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), cameraPos, cameraDir.Rotation(), spawnParam);


		if (deathCamera)
		{
			//UE_LOG(LogTemp, Log, TEXT("  asdf"));
			pc->SetViewTargetWithBlend(deathCamera, 1, EViewTargetBlendFunction::VTBlend_Linear, 0);
		}

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AACTCharacter::ShowDeathUICallBack, 1.2f);

		AUEProject05GameModeBase* gm = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
		if (gm)
		{
			gm->SetBGMusic(EBGState::EBS_Over);
		}
	}

}


void AACTCharacter::Server_PickUp_Implementation()
{
	InputPickUp();
}

bool AACTCharacter::Server_PickUp_Validate()
{
	return true;
}

//Servers


void AACTCharacter::ServerTakeWeapon_Implementation()
{
	
}

bool AACTCharacter::ServerTakeWeapon_Validate()
{
	return true;
}

void AACTCharacter::ServerFire_Implementation()
{
	
}

bool AACTCharacter::ServerFire_Validate()
{
	return true;
}

//ClimbUp
void AACTCharacter::NotifyClimbUp()
{
	bisClimbing = true;
	ILedgeInterface* animIns = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
	if (animIns)
	{
		animIns->Execute_ClimbUp(GetMesh()->GetAnimInstance(), true);
	}
}
void AACTCharacter::ClimbUp_Implementation(bool bClimbUp)
{
	bisClimbing = bClimbUp;
	bHanging = bClimbUp;
}

//HangingMove
bool AACTCharacter::HangingMoveTracer()
{
	if (MoveRightValue == 0)
	{
		return false;
	}
	UArrowComponent* arrowComp;
	if (MoveRightValue>0)
	{
		arrowComp = RightArrow;
	}
	else
	{
		arrowComp = LeftArrow;
	}

	FVector playerPos = GetActorLocation();
	FCollisionShape forwardshap;
	forwardshap.ShapeType = ECollisionShape::Capsule;
	forwardshap.Capsule.Radius = 20;
	FCollisionQueryParams forwardparam;
	forwardparam.AddIgnoredActor(this);
	FHitResult heightHit;
	FVector startPos = arrowComp->GetComponentTransform().GetLocation() - arrowComp->GetForwardVector() * 50;
	bool bCanMove;
	TArray<FHitResult> hits;
	//DrawDebugLine(GetWorld(), startPos, arrowComp->GetComponentTransform().GetLocation() + arrowComp->GetForwardVector() * 5, FColor::Red);
	//DrawDebugLine(GetWorld(), FVector(0,0,0), FVector(0, 0, 0), FColor::Red);
	//if (GetWorld()->SweepMultiByChannel(hits, startPos, arrowComp->GetComponentTransform().GetLocation() + arrowComp->GetForwardVector() * 5, FQuat(), ECC_GameTraceChannel1,forwardshap,forwardparam))
	if (GetWorld()->SweepSingleByChannel(heightHit, startPos, arrowComp->GetComponentTransform().GetLocation() + arrowComp->GetForwardVector() * 5, FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
	{
		bCanMove = true;
		//UE_LOG(LogTemp, Log, TEXT("11111111"));
	}
	else
	{
		bCanMove = false;
		//UE_LOG(LogTemp, Log, TEXT("00000000"));

	}
	if (arrowComp == LeftArrow)
	{
		bCanMoveLeft = bCanMove;
	}
	else
	{
		bCanMoveRight = bCanMove;
	}
	return bCanMove;
}


void AACTCharacter::HangingMove()
{
	HangingMoveTracer();
	if (MoveRightValue==0)
	{
		bMovingLeft = false;
		bMovingRight = false;
		return;
	}
	//if (!HangingMoveTracer())
	//{
	//	//UE_LOG(LogTemp, Log, TEXT("    HangingMove"));
	//	return;
	//}

	if (MoveRightValue>0)
	{
		//Right
		//UE_LOG(LogTemp, Log, TEXT("   Right"));
		bMovingRight = true;
		bMovingLeft = false;
	}
	else if(MoveRightValue<0)
	{
		//Left
		bMovingLeft = true;
		bMovingRight = false;
	}
	if (!IsHanging())
	{
		return;
	}
	//float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	if (!HangingMoveTracer())
	{
		//UE_LOG(LogTemp, Log, TEXT("    HangingMove"));
		HangingMoveTurnTracer();
		return;
	}
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), GetActorLocation()+ GetActorRightVector()*(MoveRightValue > 0 ? 1 : -1) * 20, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
	//FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &AACTCharacter::HangingMoveCallBack, 0.5f);
}

void AACTCharacter::MovePlayerTick(float DeltaTime)
{
	if (!bMovePlayer)
	{
		return;
	}
	if ((GetActorLocation()-MovePlayerTargetPos).Size()<=5)
	{
		MovePlayer(false, FVector());
		return;
	}
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), MovePlayerTargetPos, DeltaTime,12));
}

void AACTCharacter::MovePlayer(bool bMovePlayer, FVector targetPos)
{
	this->bMovePlayer = bMovePlayer; 
	this->MovePlayerTargetPos = targetPos;
	if (bMovePlayer)
	{
		SetPlayerState(EPlayerState::EPS_DND);
	}
	else
	{
		SetPlayerState(EPlayerState::EPS_Normal);
	}
}

//void AACTCharacter::HangingMoveCallBack()
//{
//	SetActorLocation(FMath::VInterpTo(GetActorLocation(), GetActorLocation() + GetActorRightVector()*(MoveRightValue > 0 ? 1 : -1) * 20, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
//}

//HangingMoveTurn
void AACTCharacter::HangingMoveTurnTracer()
{
	if (MoveRightValue == 0)
	{
		return ;
	}
	UArrowComponent* arrowComp;
	if (MoveRightValue > 0)
	{
		arrowComp = RightArrow;
	}
	else
	{
		arrowComp = LeftArrow;
	}
	ILedgeInterface* animIns = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
	if (!animIns) {
		return;
	}
	FVector playerPos = GetActorLocation();
	FCollisionShape forwardshap;
	forwardshap.ShapeType = ECollisionShape::Capsule;
	forwardshap.Capsule.Radius = 10;
	FCollisionQueryParams forwardparam;
	forwardparam.AddIgnoredActor(this);
	FHitResult heightHit;
	//UE_LOG(LogTemp, Log, TEXT("  JumpUpTracer"));
	//DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, 0), FColor::Red);
	FVector startPos = arrowComp->GetComponentTransform().GetLocation()+arrowComp->GetRightVector()*10*MoveRightValue;
	FVector endPos = arrowComp->GetComponentTransform().GetLocation() - arrowComp->GetRightVector() * 20 * MoveRightValue;
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red);
	if (GetWorld()->SweepSingleByChannel(heightHit, startPos, endPos, FQuat(), ECC_GameTraceChannel1, forwardshap, forwardparam))
	{
		//SetActorLocation(heightHit.Location);
		//USplineComponent
		
		if (MoveRightValue>0)
		{
			if (!bCanMoveRight)
			{
				bCanTurnRight = true;
				//UE_LOG(LogTemp, Log, TEXT("  bCanTurnRight = true;"));
			}
			//animIns->Execute_CanHangingTurn(GetMesh()->GetAnimInstance(),false, bCanTurnRight);
		}
		else
		{
			if (!bCanMoveLeft)
			{
				bCanTurnLeft = true;
			}
			//animIns->Execute_CanHangingTurn(GetMesh()->GetAnimInstance(),bCanTurnLeft, false);
		}
		HangingTurnTestVec = heightHit.Location;
		HangingTurnNormal = heightHit.Normal*-1;
		FVector turnToPos = FVector(heightHit.Location.X+heightHit.Normal.X*30, heightHit.Location.Y + heightHit.Normal.Y * 30,GetActorLocation().Z);
		HangingTurn(turnToPos);
	}
	else
	{
		if (MoveRightValue > 0)
		{
			bCanTurnRight = false;
		}
		else
		{
			bCanTurnLeft = false;
		}
	}
	//animIns->Execute_CanHangingTurn(GetMesh()->GetAnimInstance(), bCanTurnLeft, bCanTurnRight);
}

void AACTCharacter::HangingTurn(const FVector& TurnToVec)
{
	if (bIsHangingTurn)
	{
		return;
	}
	bIsHangingTurn = true;
	
	TurnSplineComp = GetWorld()->SpawnActor<AACTSplineTurn>(AACTSplineTurn::StaticClass(), GetActorTransform())->GetSplineTurnComp();
	TurnSplineComp->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World);
	TurnSplineComp->SetLocationAtSplinePoint(1, TurnToVec, ESplineCoordinateSpace::World);
	TurnSplineComp->SetTangentAtSplinePoint(0, FVector(0, 200, 0), ESplineCoordinateSpace::Local);
	TurnSplineComp->SetTangentAtSplinePoint(1, FVector(200,0, 0), ESplineCoordinateSpace::Local);
	TurnSplineLength= TurnSplineComp->GetSplineLength();

}

void AACTCharacter::HangingTurnTick(float DeltaTime)
{
	if (!bIsHangingTurn|| !TurnSplineComp)
	{
		return;
	}
	
	if (TurnSplineDeltaTime>=TurnSplineMaxTime)
	{
		bIsHangingTurn = false;
		TurnSplineDeltaTime = 0;
		TurnSplineComp->GetOwner()->Destroy();
		TurnSplineComp = nullptr;
		bCanTurnLeft = false;
		bCanTurnRight = false;
		SetActorRotation(FRotator(GetActorRotation().Pitch, HangingTurnNormal.Rotation().Yaw, GetActorRotation().Roll));
		//ILedgeInterface* animIns = Cast<ILedgeInterface>(GetMesh()->GetAnimInstance());
		//if (animIns)
		//{
		//	animIns->Execute_CanHangingTurn(GetMesh()->GetAnimInstance(),false,false);
		//}
		return;
	}

	float splineLength = TurnSplineLength*(TurnSplineDeltaTime/TurnSplineMaxTime);
	FVector playerPos= TurnSplineComp->GetLocationAtDistanceAlongSpline(splineLength, ESplineCoordinateSpace::World);
	SetActorLocation(playerPos);
	TurnSplineDeltaTime += DeltaTime;
	FRotator playerRotaor= FMath::RInterpTo(GetActorRotation(), HangingTurnNormal.Rotation(), DeltaTime, 3);
	SetActorRotation(FRotator(GetActorRotation().Pitch,playerRotaor.Yaw,GetActorRotation().Roll));
	//UE_LOG(LogTemp, Log, TEXT(""));
}





void AACTCharacter::InputDoViewTask()
{
	//找HUD，让HUD取帮助我们管理UI
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (!pc)
	{
		return;
	}
	AACTHUD* hud = Cast<AACTHUD>(pc->GetHUD());
	if (!hud)
	{
		return;
	}
	if (!bFindTask||bReceiveTask)
	{
		return;
	}
	hud->ViewTask();


}
//Roll
void AACTCharacter::DoRoll()
{
	bRoll = true;
}

void AACTCharacter::DoUnRoll()
{
	bRoll = false;
}

void AACTCharacter::DoShowTask()
{
	AACTHUD* hud = Cast<AACTHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!hud)
	{
		return;
	}
	if (hud->GetMainUi())
	{
		if (hud->GetMainUi()->GetShowTask())
		{
			hud->GetMainUi()->GetShowTask()->OnPackUp();
		}
	}
	
}

void AACTCharacter::InputEscape()
{

}
