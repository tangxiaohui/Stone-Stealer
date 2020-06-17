// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_ClearValue.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"



void UBTTaskNode_ClearValue::InitializeFromAsset(UBehaviorTree &Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBdata = GetBlackboardAsset();
	if (BBdata)
	{
		BlackBoardKey.ResolveSelectedKey(*BBdata);
	}
}

EBTNodeResult::Type UBTTaskNode_ClearValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->ClearValue(BlackBoardKey.GetSelectedKeyID());
	return EBTNodeResult::Succeeded;
}
