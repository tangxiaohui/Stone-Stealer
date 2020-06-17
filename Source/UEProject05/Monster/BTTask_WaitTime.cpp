// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_WaitTime.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UBTTask_WaitTime::UBTTask_WaitTime()
{
	WaitTimeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_WaitTime, WaitTimeKey));
	WaitTime = 3;
	bUserBlackBoardTime = false;
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_WaitTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bUserBlackBoardTime)
	{
		
		WaitTimeTick = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Float>(WaitTimeKey.GetSelectedKeyID());
	}
	else
	{
		WaitTimeTick = WaitTime;
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitTime::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData)
	{
		WaitTimeKey.ResolveSelectedKey(*BBData);
	}
}

void UBTTask_WaitTime::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	WaitTimeTick -= DeltaSeconds;
	if (WaitTimeTick<=0)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
