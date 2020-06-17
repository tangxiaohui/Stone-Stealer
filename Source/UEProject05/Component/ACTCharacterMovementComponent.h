// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ACTCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UACTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UACTCharacterMovementComponent();

	
protected:
	virtual float GetMaxSpeed() const override;


protected:
	UPROPERTY(EditAnywhere)
		float NormalSpeed;
	UPROPERTY(EditAnywhere)
		float NormalRunSpeed;
	UPROPERTY(EditAnywhere)
		float HoldSpeed;
	UPROPERTY(EditAnywhere)
		float HoldRunSpeed;
	UPROPERTY(EditAnywhere)
		float CrouchSpeed;

};
