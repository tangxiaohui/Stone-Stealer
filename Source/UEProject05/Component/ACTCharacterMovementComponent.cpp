// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTCharacterMovementComponent.h"
#include "Player/ACTCharacter.h"

UACTCharacterMovementComponent::UACTCharacterMovementComponent()
{
	NormalSpeed=200;
	NormalRunSpeed=800;
	HoldSpeed=400;
	HoldRunSpeed=800;
	CrouchSpeed=150;
}

float UACTCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = NormalSpeed;

	AACTCharacter* player = Cast<AACTCharacter>(GetOwner());
	if (!player)
	{
		return MaxSpeed;
	}
	if (player->IsHoldWeapon())
	{
		MaxSpeed = HoldSpeed;
	}

	if (player->IsSpring())
	{
		if (player->IsHoldWeapon())
		{
			MaxSpeed = HoldRunSpeed;
		}
		else
		{
			MaxSpeed =NormalRunSpeed;
		}
	}

	if (player->IsCrouch())
	{
		MaxSpeed = CrouchSpeed;
	}




	return MaxSpeed;

}
