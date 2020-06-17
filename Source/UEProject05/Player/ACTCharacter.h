// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/LedgeInterface.h"
#include "ACTCharacter.generated.h"



UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Normal,
	EPS_DND,
	EPS_Death,
};


class UAnimMontage;
class AWeaponBasic;

UCLASS()
class UEPROJECT05_API AACTCharacter : public ACharacter,public ILedgeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACTCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /* = 0 */) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	//Component
	UPROPERTY(VisibleAnywhere)
	class UACTIKSystemComp* IKSystemComp;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* MainCamera;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SprintArmComp;
	UPROPERTY(VisibleAnywhere)
	class UPawnNoiseEmitterComponent* playerNoise;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UArrowComponent* LeftArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* RightArrow;

	//Weapon
	UPROPERTY(EditAnywhere,Replicated)
	AWeaponBasic* HoldWeapon;
	UPROPERTY(EditAnywhere, Replicated)
	AWeaponBasic*	LeftWeapon;
	UPROPERTY(EditAnywhere, Replicated)
	AWeaponBasic* RightWeapon;
	UPROPERTY(EditAnywhere, Replicated)
	AWeaponBasic* RightSword;
	UPROPERTY(Replicated)
	AWeaponBasic* ChangeWeapon;

	//Spawn
	UPROPERTY(EditAnywhere)
	class AACTSplineTurn* TurnSpline;
	UPROPERTY(EditAnywhere)
	class USplineComponent* TurnSplineComp;

	//NPC
	UPROPERTY(EditAnywhere)
	class ANPC_Character* TaskNPC;

	//Resource
	UPROPERTY(EditAnywhere)
	UAnimMontage* EquipSwordMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* UnequipSwordMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DamageForwardMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DamageBackMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DamageLeftMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DamageRightMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	USoundBase* DamgeSound;

	EPlayerState PlayerState;


	//Movement
	bool bSpring;
	bool bCrouch;
	bool bIronSights;
	bool bJump;
	float PlayerHP;
	float PlayerMaxHP;

	float aimDistance;
	//Task
	bool bFindTask;
	bool bReceiveTask;

	//DNDTimer
	float ReleaseDNDTime;

	//Roll
	bool bRoll;

	//Ldege
	
	bool bHanging;
	bool bSlideTrace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ledge Param")
	bool bFreeHanging;
	bool bCanJumpUp;
	bool bisClimbing;
	bool bCanMoveLeft;
	bool bCanMoveRight;
	bool bMovingLeft;
	bool bMovingRight;
	bool bCanTurnLeft;
	bool bCanTurnRight;
	bool bIsHangingTurn;
	bool bMovePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveRightValue;
	float MoveForwardValue;
	float TurnSplineLength;
	float TurnSplineMaxTime;
	float TurnSplineDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector wallNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector wallLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector wallHeightLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator wallRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector JumpUpLaunchVec;

	FVector HangingTurnNormal;
	FVector MovePlayerTargetPos;


	//Test
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector JumpUpTestVec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector HangingTurnTestVec;

public:
	bool IsSpring() const;
	bool IsCrouch() const { return bIsCrouched; }


	class USpringArmComponent* GetSpringArmComp() const { return SprintArmComp; }
	class UCameraComponent* GetMainCameraComp() const { return MainCamera; }

	//Weapon
	bool IsHoldWeapon() const { return HoldWeapon != nullptr; }
	bool IsIronSights() const { return bIronSights; }
	float GetIronsightsMove() const { return aimDistance; }
	bool IsHoldSword() const;
	void NotifyEquipSword();
	void NotifyUnequipSword();
	AWeaponBasic* GetHoldingWeapon() const { return HoldWeapon; }

	//Noise

	void PlayerMakeNoise(float Loudness, float MaxRange);

	//Ledge
	bool IsFreeHanging() const { return bFreeHanging; }
	bool IsHanging() const { return bHanging; }
	FVector GetJumpUpLaunchVec() const { return JumpUpLaunchVec; }
	void ReleaseLedge();
	//void IsCanHangingMove(bool& bCanMoveLeft, bool& bCanMoveRright, bool& bMovingLeft, bool& bMovingRight) { bCanMoveLeft = this->bCanMoveLeft; bCanMoveRight = this->bCanMoveRight; bMovingLeft = this->bMovingLeft; bMovingRight = this->bMovingRight; }
	bool IsCanMoveLeft() { return bCanMoveLeft; }
	bool IsCanMoveRight() { return bCanMoveRight; }
	bool IsMovingLeft() { return bMovingLeft; }
	bool IsMovingRight() { return bMovingRight; }
	bool IsCanTurnLeft() { return bCanTurnLeft; }
	bool IsCanTurnRight() { return bCanTurnRight; }

	//Task
	void HaveFindTask() {bFindTask = true;}
	void NoFindTask() {bFindTask = false;}
	void ReceiveTask() { bReceiveTask = true; }
	bool GetIsTask() { return bReceiveTask; }
	void SetTaskNPC(class ANPC_Character* TaskNPC) { this->TaskNPC = TaskNPC; }
	class ANPC_Character* GetTaskNPC() {return TaskNPC; }

	//Roll
	bool IsRoll() { return bRoll; }

	//Movement
	float GetMoveRightValue() const { return MoveRightValue; }
	float GetMoveForwardValue() const { return MoveForwardValue; }
	void SetPlayerState(EPlayerState state) { PlayerState = state; }

protected:
	//Input
	//Axis
	void InputMoveForward(float DeltaTime);
	void InputMoveRight(float DeltaTime);
	void InputTurnUp(float DeltaTime);
	void InputTurnRight(float DeltaTime);
	//Action
	void InputDoJump();
	void InputDoStopJump();
	void DoBeingAttack();
	void DoStopAttack();
	void InputDoSpring();
	void InputDoUnSpring();
	void InputDoCrouch();
	void InputDoUnCrouch();
	void InputPickUp();
	void InputThrowAway();
	void InputKey_1();
	void InputKey_2();
	void InputKey_3();
	void DoIronsight();
	void InputDoViewTask();
	void DoRoll();
	void DoUnRoll();
	void DoShowTask();
	void InputEscape();


	//Weapon
	void TakeWeapon(AWeaponBasic* weapon);
	void DoHoldWeapon(AWeaponBasic* weapon);
	void DoThrowWeapon(AWeaponBasic* weapon);
	void HoldOnWeapon(AWeaponBasic* weapon);
	void IronsightsMoveTick(float DeltaTime);
	void EventKeyTakeWeapon(AWeaponBasic* weapon,FName slotName);
	void ChangeWeaponCallBack();
	

	//Timer
	void ReleaseDNDTimer(float time);
	void ReleaseDNDTimerCallBack();

	//Montage
	void PlayMontage_Player(UAnimMontage* montage);

	//Ledge
	void LedgeTick(float DeltaTime);
	void GrabLedge();
	void HangingMove();
	//void HangingMoveCallBack();
	void MovePlayerTick(float DeltaTime);
	void MovePlayer(bool bMovePlayer, FVector targetPos);
	
	void ForwardTracer(float DeltaTime);
	void WallHeightTracer(float DeltaTime);
	void JumpTracer();
	void JumpUpTracer();
	bool HangingMoveTracer();
	void HangingMoveTurnTracer();
	void HangingTurn(const FVector& TurnToVec);
	void HangingTurnTick(float DeltaTime);

	void NotifyJumpUp();
	void NotifyClimbUp();

	virtual void ClimbUp_Implementation(bool bClimbUp) override;
	virtual void JumpUp_Implementation(bool bClimbUp) override;

	//UI
	void SetHPWidget();
	void ShowDeathUICallBack();
	//Death
	void DeathCallBack();

	//Server
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTakeWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PickUp();

};
