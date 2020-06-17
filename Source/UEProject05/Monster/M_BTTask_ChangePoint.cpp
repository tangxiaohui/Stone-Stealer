// Fill out your copyright notice in the Description page of Project Settings.

#include "M_BTTask_ChangePoint.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Object.h"
#include "NavigationSystem.h"
#include "AIController.h"

UM_BTTask_ChangePoint::UM_BTTask_ChangePoint()
{
	OriginPosKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UM_BTTask_ChangePoint, OriginPosKey));
	OriginPosKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UM_BTTask_ChangePoint, OriginPosKey), AActor::StaticClass());
	TargetPosKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UM_BTTask_ChangePoint, TargetPosKey));
}

EBTNodeResult::Type UM_BTTask_ChangePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector StartPos;
	if (OriginPosKey.SelectedKeyType==UBlackboardKeyType_Vector::StaticClass())
	{
		//UBlackboardComponent
		StartPos = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(OriginPosKey.GetSelectedKeyID());
	}
	else
	{
		UObject* Obj = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(OriginPosKey.GetSelectedKeyID());
		AActor* act = Cast<AActor>(Obj);
		if (act)
		{
			StartPos = act->GetActorLocation();
		}
	}
	FVector FindPos;

	OwnerComp.GetAIOwner();
	UNavigationSystemV1::K2_GetRandomPointInNavigableRadius(OwnerComp.GetAIOwner(), StartPos, FindPos, FMath::FRandRange(RandomMin, RandomMax));
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(TargetPosKey.GetSelectedKeyID(), FindPos);
	return EBTNodeResult::Succeeded;

}

void UM_BTTask_ChangePoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBdata = GetBlackboardAsset();
	if (BBdata)
	{
		TargetPosKey.ResolveSelectedKey(*BBdata);
		OriginPosKey.ResolveSelectedKey(*BBdata);
	}
}
