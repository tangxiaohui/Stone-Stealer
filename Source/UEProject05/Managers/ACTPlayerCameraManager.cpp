// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTPlayerCameraManager.h"
#include "Player/ACTCharacter.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

void AACTPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	//player = Cast<AACTCharacter>(GetOwningPlayerController()->GetCharacter());
	player = Cast<AACTCharacter>(PCOwner->GetPawn());
	if (!player)
	{
		return;
	}

	DoCrouch(DeltaTime);
	HoldWeapon(DeltaTime);
	DoIronsights(DeltaTime);

	Super::UpdateCamera(DeltaTime);
}

void AACTPlayerCameraManager::DoCrouch(float DeltaTime)
{
	if (player->IsCrouch()&&!bCrouch)
	{
		player->GetSpringArmComp()->SetRelativeLocation(FVector(0,0,28));
	}
	else if(!player->IsCrouch() && bCrouch)
	{
		player->GetSpringArmComp()->SetRelativeLocation(FVector(0, 0, -28));
	}
	bCrouch = player->IsCrouch();

	float cameraY = FMath::FInterpTo(player->GetSpringArmComp()->GetRelativeTransform().GetLocation().Z, 0, DeltaTime, 8);
	player->GetSpringArmComp()->SetRelativeLocation(FVector(0,0, cameraY));
}

void AACTPlayerCameraManager::HoldWeapon(float DeltaTime)
{
	float toZ = 0;
	float soY = 0;
	float riX = 0;
	if (player->IsHoldWeapon())
	{
		if (player->IsHoldSword())
		{
			toZ = FMath::FInterpTo(player->GetSpringArmComp()->TargetOffset.Z, 50, DeltaTime, 3);
			soY = FMath::FInterpTo(player->GetSpringArmComp()->SocketOffset.Y, 40, DeltaTime, 3);
			riX = FMath::FInterpTo(player->GetMainCameraComp()->GetRelativeTransform().GetLocation().X, 100, DeltaTime, 3);
		}
		else
		{
			toZ = FMath::FInterpTo(player->GetSpringArmComp()->TargetOffset.Z, 100, DeltaTime, 8);
			soY = FMath::FInterpTo(player->GetSpringArmComp()->SocketOffset.Y, 85, DeltaTime, 8);
			riX = FMath::FInterpTo(player->GetMainCameraComp()->GetRelativeTransform().GetLocation().X, 220, DeltaTime, 8);
		}
	}
	else
	{
		toZ = FMath::FInterpTo(player->GetSpringArmComp()->TargetOffset.Z, 0, DeltaTime, 8);
		soY = FMath::FInterpTo(player->GetSpringArmComp()->SocketOffset.Y, 0, DeltaTime, 8);
		riX = FMath::FInterpTo(player->GetMainCameraComp()->GetRelativeTransform().GetLocation().X, 0, DeltaTime, 8);
	}

	player->GetSpringArmComp()->TargetOffset = FVector(0, 0, toZ);
	player->GetSpringArmComp()->SocketOffset = FVector(0, soY, 0);
	player->GetMainCameraComp()->SetRelativeLocation(FVector(riX, 0, 0));
}

void AACTPlayerCameraManager::DoIronsights(float DeltaTime)
{
	if (!player->IsHoldWeapon())
	{
		return;
	}
	float FOV = 0;
	if (player->IsIronSights())
	{
		FOV = 50;
	}
	else
	{
		FOV = 90;
	}
	DefaultFOV = FMath::FInterpTo(DefaultFOV, FOV, DeltaTime, 8);
	SetFOV(DefaultFOV);
}
