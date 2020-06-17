// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_PatrolArea.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTDecorator_PatrolArea : public UBTDecorator
{
	GENERATED_BODY()
		UBTDecorator_PatrolArea();
protected:
	virtual	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;

	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;


protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OriginKey;
	UPROPERTY(EditAnywhere)
	float Raduis;
};
