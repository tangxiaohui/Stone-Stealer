// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_AnimInstance.h"
#include "NPC_Character.h"

void UNPC_AnimInstance::NativeBeginPlay()
{
	bFind = false;
}

void UNPC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ANPC_Character* NPC = Cast<ANPC_Character>(TryGetPawnOwner());

	if (!NPC)
	{
		return;
	}
	bFind = NPC->GetFind();
	if (!bFind)
	{

		if ((TickValue += DeltaSeconds) >= 2)
		{
			bNormalIdle = true;

		}
		else
		{
			bNormalIdle = false;
		}
	}
	else
	{
		TickValue = 0;
		bNormalIdle = false;
	}
}


