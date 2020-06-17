// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Montage.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UBTTaskNode_Montage : public UBTTaskNode
{
	GENERATED_BODY()
		UBTTaskNode_Montage();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset);

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	void PlayMontageTimeEnd();

protected:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;//需要播放的动画
	UPROPERTY(EditAnywhere)
		bool bSelfPlay;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bSelfPlay"))
		FBlackboardKeySelector PlayMontageActor;
	UPROPERTY()
		class UBehaviorTreeComponent* BTreeComp;//当角色被消灭时，需要把角色行为树存一个指针
	FTimerHandle handle;//定时句柄
};
