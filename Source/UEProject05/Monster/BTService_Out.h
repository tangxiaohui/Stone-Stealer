// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Out.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTService_Out : public UBTService
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
protected:
	UPROPERTY(EditAnywhere)
		float ArceValues;
	UPROPERTY()
		class AACTCharacter* Player;
};
