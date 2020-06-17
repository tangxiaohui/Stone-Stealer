// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPC_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UNPC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeBeginPlay()override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
protected:
	UPROPERTY(BlueprintReadOnly)
		uint8 bFind : 1;
	UPROPERTY(BlueprintReadOnly)
		uint8 bNormalIdle : 1;
	float TickValue;
};
