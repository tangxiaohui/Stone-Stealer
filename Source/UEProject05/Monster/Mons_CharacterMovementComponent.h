// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Mons_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UMons_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UMons_CharacterMovementComponent();
protected:
	virtual float  GetMaxSpeed()const override;
private:
	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
		float SpringSpeed;//³å´ÌËÙ¶È

	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
		float AddSpeed;
};
