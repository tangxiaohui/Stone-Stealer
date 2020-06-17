// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ACTPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API AACTPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

protected:
	virtual void UpdateCamera(float DeltaTime) override;



protected:
	class AACTCharacter* player;

	bool bCrouch;

protected:
	void DoCrouch(float DeltaTime);
	void HoldWeapon(float DeltaTime);
	void DoIronsights(float DeltaTime);
};
