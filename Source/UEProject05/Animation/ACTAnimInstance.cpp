// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTAnimInstance.h"
#include "Player/ACTCharacter.h"
#include <GameFramework/PawnMovementComponent.h>
#include <Components/CapsuleComponent.h>

void UACTAnimInstance::NativeBeginPlay()
{
	bHoldWeapon = false;
	bNormal = true;
	bCrouch = false;
	bIronSights = false;
	bJump = false;
	bFire = false;
	
}

void UACTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	player = Cast<AACTCharacter>(TryGetPawnOwner());
	if (!player)
	{
		return;
	}

	//速度
	Speed = player->GetVelocity().Size2D();
	//持枪方向
	Direction = CalculateDirection(player->GetVelocity(), player->GetActorRotation());
	//蹲
	bCrouch = player->IsCrouch();
	//拿武器
	bHoldWeapon = player->IsHoldWeapon();
	//是不是剑
	bSword = player->IsHoldSword();
	//Aim
	FVector pcDir = player->GetBaseAimRotation().Vector();
	FVector pDir = player->ActorToWorld().InverseTransformVectorNoScale(pcDir);
	FRotator pRot = pDir.Rotation();
	RotPitch = pRot.Pitch;
	RotYaw = pRot.Yaw;

	//Roll
	bRoll = player->IsRoll();

	//Ironsights
	bIronSights = player->IsIronSights();

	//Normal
	if (player->GetVelocity().Size() <= 0.2 && !player->IsHoldWeapon())
	{
		if ((norTime += DeltaSeconds) >= norTimeLength)
		{
			bNormal = true;
		}
	}
	else
	{
		bNormal = false;
		norTime = 0;
	}
	
	//air
	bIsInAir = player->GetMovementComponent()->IsFalling();

	//Ledge
	//FreeHang
	bFreeHanging = player->IsFreeHanging();
	//HangingCanMove
	bCanMoveLeft = player->IsCanMoveLeft();
	bCanMoveRight = player->IsCanMoveRight();
	bMovingLeft = player->IsMovingLeft();
	bMovingRight = player->IsMovingRight();
	bCanHangingTurnLeft = player->IsCanTurnLeft();
	bCanHangingTurnRight = player->IsCanTurnRight();
	//Movement
	MoveRightValue = player->GetMoveRightValue();
	MoveForwardValue = player->GetMoveForwardValue();



}

void UACTAnimInstance::AnimNotify_ChangeNormal()
{
	bNormal = false;
	norTime = 0;
	NorIndex = FMath::RandRange(0, 1);
}

//ClimbUp
void UACTAnimInstance::ClimbUp_Implementation(bool bClimbUp)
{
	this->bClimbUp = bClimbUp;
}
void UACTAnimInstance::AnimNotify_ClimbUp_Begin()
{
	bIsHanging = false;
	AACTCharacter* player=Cast<AACTCharacter>(TryGetPawnOwner()) ;
	if (!player)
	{
		return;
	}
	player->LaunchCharacter(FVector(player->GetActorForwardVector().X *100, player->GetActorForwardVector().Y * 100, 600),false,false);
	//UE_LOG(LogTemp, Log, TEXT("  ClimbUp_Begin"));
	player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void UACTAnimInstance::AnimNotify_ClimbUp_End()
{
	bClimbUp = false;
	//UE_LOG(LogTemp, Log, TEXT("  ClimbUp_End"));
	ILedgeInterface* playerI = Cast<ILedgeInterface>(TryGetPawnOwner());
	if (playerI)
	{
		playerI->Execute_ClimbUp(TryGetPawnOwner(),false);
	}
	player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

//JumpUp
void UACTAnimInstance::JumpUp_Implementation(bool bJumpUp)
{
	this->bJumpUp = bJumpUp;
}

//void UACTAnimInstance::CanHangingTurn_Implementation(bool bCanTurnRight, bool bCanTurnLeft)
//{
//	bCanHangingTurnLeft = bCanTurnLeft;
//	bCanHangingTurnRight = bCanTurnRight;
//}

void UACTAnimInstance::AnimNotify_JumpUp_Begin()
{
	AACTCharacter* player = Cast<AACTCharacter>(TryGetPawnOwner());
	if (!player)
	{
		return;
	}
	player->LaunchCharacter(player->GetJumpUpLaunchVec(), true, true);
	player->ReleaseLedge();
	//UE_LOG(LogTemp, Log, TEXT("  Run JumpUp : %f"), player->GetJumpUpLaunchVec().Size());
}
void UACTAnimInstance::AnimNotify_JumpUp_End()
{
	bJumpUp = false;
	ILedgeInterface* playerI = Cast<ILedgeInterface>(TryGetPawnOwner());
	if (playerI)
	{
		playerI->Execute_JumpUp(TryGetPawnOwner(), false);
	}
}


void UACTAnimInstance::AnimNotify_EquipSword()
{
	AACTCharacter* player = Cast<AACTCharacter>(TryGetPawnOwner());
	if (!player)
	{
		return;
	}
	player->NotifyEquipSword();
}

void UACTAnimInstance::AnimNotify_UnequipSword()
{
	AACTCharacter* player = Cast<AACTCharacter>(TryGetPawnOwner());
	if (!player)
	{
		return;
	}
	player->NotifyUnequipSword();
}

void UACTAnimInstance::CanGrab_Implementation(bool bCanGroub)
{
	bIsHanging = bCanGroub;
}

void UACTAnimInstance::AnimNotiFy_PlayNoise()
{
	player->PlayerMakeNoise(1, 1);
}

