// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_TrunBack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Monster_Character.h"

UBTService_TrunBack::UBTService_TrunBack()
{
	ArceValue = 3000;
}

void UBTService_TrunBack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AMonster_Character* Mon = Cast<AMonster_Character>(OwnerComp.GetAIOwner()->GetPawn());
	APawn* Player = OwnerComp.GetAIOwner()->GetPawn();

	if (!Player)
	{
		return;
	}

	FVector StartPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("StartPos"));
	if( (Player->GetActorLocation()-StartPos).Size2D()>ArceValue)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		Mon->NotSpring();
		Mon->GetAlarm()->Destroy();

	}
}
