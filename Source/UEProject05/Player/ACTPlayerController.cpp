// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTPlayerController.h"
#include "Managers/ACTPlayerCameraManager.h"

AACTPlayerController::AACTPlayerController()
{
	PlayerCameraManagerClass = AACTPlayerCameraManager::StaticClass();
}
