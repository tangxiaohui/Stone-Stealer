// Fill out your copyright notice in the Description page of Project Settings.

#include "Mons_CharacterMovementComponent.h"
#include "Monster_Character.h"

UMons_CharacterMovementComponent::UMons_CharacterMovementComponent()
{
	SpringSpeed = 600;
	AddSpeed = 200;
}

float UMons_CharacterMovementComponent::GetMaxSpeed() const
{
	Super::GetMaxSpeed();
	AMonster_Character* Mon = Cast<AMonster_Character>(GetOwner());
	if (Mon)
	{
		if (Mon->IsSpring())
		{
			return SpringSpeed;
		}
		else
		{
			return AddSpeed;
		}
		
	}
	return AddSpeed;
}
