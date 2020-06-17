// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TrunBack.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTService_TrunBack : public UBTService
{
	GENERATED_BODY()
		UBTService_TrunBack();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
protected:
	UPROPERTY(EditAnywhere)
		float ArceValue;

};
