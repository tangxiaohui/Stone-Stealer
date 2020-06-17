// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "M_BTTask_ChangePoint.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UM_BTTask_ChangePoint : public UBTTaskNode
{
	GENERATED_BODY()
		UM_BTTask_ChangePoint();
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector OriginPosKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetPosKey;
	UPROPERTY(EditAnywhere)
	float RandomMax;
	UPROPERTY(EditAnywhere)
	float RandomMin;

};
