// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitTime.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTTask_WaitTime : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTask_WaitTime();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void InitializeFromAsset(UBehaviorTree& Asset)override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

protected:

	UPROPERTY(EditAnywhere, meta = (EditCondition="!bUserBlackBoardTime"))
		float WaitTime;
	UPROPERTY(EditAnywhere,meta = (EditCondition = "bUserBlackBoardTime"))
	FBlackboardKeySelector WaitTimeKey;
	UPROPERTY(EditAnywhere)
	bool bUserBlackBoardTime;

	float WaitTimeTick;
};
