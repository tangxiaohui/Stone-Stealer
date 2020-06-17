// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Monster_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UMonster_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeBeginPlay()override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
public:
	UFUNCTION()
		void AnimNotify_MoveStep();
	
protected:
	UPROPERTY(BlueprintReadOnly)
		float Speed;
	UPROPERTY(BlueprintReadOnly)
		uint8 bNormalIdle : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bDead : 1;
	UPROPERTY(BlueprintReadOnly)
		int32 NormalPosIndex;
	UPROPERTY(BlueprintReadOnly)
		uint8 bHoldGun : 1;
	UPROPERTY(BlueprintReadOnly)
		float Pich;
	float TickValue;
};
