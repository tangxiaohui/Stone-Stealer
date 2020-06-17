// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ClearValue.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTTaskNode_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void InitializeFromAsset(UBehaviorTree &Asset)override;
protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector BlackBoardKey;
};
