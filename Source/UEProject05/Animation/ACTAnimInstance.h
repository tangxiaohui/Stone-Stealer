// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/LedgeInterface.h"
#include "ACTAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UACTAnimInstance : public UAnimInstance,public ILedgeInterface
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(BlueprintReadOnly)
		float Speed;
	UPROPERTY(BlueprintReadOnly)
		float Direction;
	UPROPERTY(BlueprintReadOnly)
		float RotPitch;
	UPROPERTY(BlueprintReadOnly)
		float RotYaw;
	UPROPERTY(BlueprintReadOnly)
		float MoveRightValue;
	UPROPERTY(BlueprintReadOnly)
		float MoveForwardValue;
	UPROPERTY(BlueprintReadOnly)
		int32 NorIndex;
	UPROPERTY(BlueprintReadOnly)
		uint8 bHoldWeapon : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bNormal : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bCrouch : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bIronSights : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bJump : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bFire : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bSword : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bIsInAir : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bIsHanging : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bFreeHanging : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bClimbUp : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bJumpUp : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bCanMoveLeft : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bCanMoveRight : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bMovingLeft : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bMovingRight : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bCanHangingTurnLeft : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bCanHangingTurnRight : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bRoll : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bIsDeath : 1;

private:
	float norTime;
	float norTimeLength;

	UPROPERTY()
	class AACTCharacter* player;


public:

	void SetDeath(bool bDeath) { bIsDeath = bDeath; }



protected:
	UFUNCTION()
	void AnimNotify_ChangeNormal();
	UFUNCTION()
	void AnimNotify_ClimbUp_Begin();
	UFUNCTION()
	void AnimNotify_ClimbUp_End();
	UFUNCTION()
		void AnimNotify_JumpUp_Begin();
	UFUNCTION()
		void AnimNotify_JumpUp_End();
	UFUNCTION()
		void AnimNotify_EquipSword();
	UFUNCTION()
		void AnimNotify_UnequipSword();
	UFUNCTION()
		void AnimNotiFy_PlayNoise();

	virtual void CanGrab_Implementation(bool bCanGroub) override;
	virtual void ClimbUp_Implementation(bool bClimbUp) override;
	virtual void JumpUp_Implementation(bool bJumpUp) override;
	//virtual void CanHangingTurn_Implementation(bool bCanTurnRight, bool bCanTurnLeft) override;
};
