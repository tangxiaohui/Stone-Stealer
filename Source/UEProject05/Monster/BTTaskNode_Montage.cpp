// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_Montage.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

UBTTaskNode_Montage::UBTTaskNode_Montage()
{
	bSelfPlay = true;
	PlayMontageActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_Montage, PlayMontageActor), APawn::StaticClass());
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTaskNode_Montage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!Montage)//需要播放的动画
	{
		return EBTNodeResult::Failed;
	}
	if (handle.IsValid())
	{
		handle.Invalidate();
	}
	UAnimInstance* Instance = nullptr;
	APawn* pawn = nullptr;
	if (bSelfPlay)
	{
		pawn = OwnerComp.GetAIOwner()->GetPawn();// AAIController里面有GetAIOwner()；
	}
	else
	{
		
		pawn =Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(PlayMontageActor.GetSelectedKeyID()));
	}
	if (!pawn)
	{
		return EBTNodeResult::Failed;
	}
	USkeletalMeshComponent* Mash = Cast<USkeletalMeshComponent>(pawn->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (!Mash)
	{
		return EBTNodeResult::Failed;
	}
	Instance = Mash->GetAnimInstance();
	if (!Instance)
	{
		return EBTNodeResult::Failed;
	}
	float PlayTime = Instance->Montage_Play(Montage);

	OwnerComp.GetAIOwner()->GetWorld()->GetTimerManager().SetTimer(handle, this, &UBTTaskNode_Montage::PlayMontageTimeEnd, PlayTime);
	BTreeComp = &OwnerComp;////当角色被消灭时，需要把角色行为树存一个指针,必须是引用而不是赋值，因为要互相影响
	return EBTNodeResult::InProgress;//让任务停滞
}

void UBTTaskNode_Montage::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBBata = GetBlackboardAsset();
	if (BBBata)
	{
		PlayMontageActor.ResolveSelectedKey(*BBBata);
	}
}

EBTNodeResult::Type UBTTaskNode_Montage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (handle.IsValid())
	{
		OwnerComp.GetAIOwner()->GetWorld()->GetTimerManager().ClearTimer(handle);
	}
	return Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_Montage::PlayMontageTimeEnd()
{
	FinishLatentTask(*BTreeComp, EBTNodeResult::Succeeded);
}
