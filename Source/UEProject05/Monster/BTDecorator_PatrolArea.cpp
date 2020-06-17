// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_PatrolArea.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

UBTDecorator_PatrolArea::UBTDecorator_PatrolArea()
{
	Raduis = 1000;
	OriginKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_PatrolArea, OriginKey));
	OriginKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_PatrolArea, OriginKey), AActor::StaticClass());
}

bool UBTDecorator_PatrolArea::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//AAIController
	APawn* pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!pawn)
	{
		return false;
	}

	FVector OriginPos;
	if (OriginKey.SelectedKeyType==UBlackboardKeyType_Vector::StaticClass())
	{
		
		OriginPos = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(OriginKey.GetSelectedKeyID());
	}
	else
	{
		UObject* obj = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(OriginKey.GetSelectedKeyID());
		AActor* act = Cast<AActor>(obj);
		if (act)
		{
			OriginPos = act->GetActorLocation();
		}
	}
	/*if ((pawn->GetActorLocation() - OriginPos).Size()<Raduis)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPos"), OriginPos);
	}*/
	return (pawn->GetActorLocation() - OriginPos).Size()<Raduis;//ÊÇ·ñ³¬¹ý·¶Î§
}


void UBTDecorator_PatrolArea::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBdata = GetBlackboardAsset();
	if (BBdata)
	{
		OriginKey.ResolveSelectedKey(*BBdata);
	}
}
