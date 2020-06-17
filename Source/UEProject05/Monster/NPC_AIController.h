// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual	void Possess(APawn* InPawn)override;
	virtual void UnPossess();
};
