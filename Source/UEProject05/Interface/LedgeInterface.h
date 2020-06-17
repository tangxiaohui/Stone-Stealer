// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LedgeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULedgeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UEPROJECT05_API ILedgeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void CanGrab(bool bCanGroub);
	UFUNCTION(BlueprintNativeEvent)
	void ClimbUp(bool bClimbup);
	UFUNCTION(BlueprintNativeEvent)
	void JumpUp(bool bJumpUp);
	//UFUNCTION(BlueprintNativeEvent)
	//void CanHangingTurn(bool bCanTurnRight,bool bCanTurnLeft);
};
