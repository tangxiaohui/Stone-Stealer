// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_AIController.h"
#include "NPC_Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BrainComponent.h"

void ANPC_AIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	ANPC_Character* NPC = Cast<ANPC_Character>(InPawn);
	if (!NPC)
	{
		return;
	}
	RunBehaviorTree(NPC->GetBehaviorTree());
}

void ANPC_AIController::UnPossess()
{
	Super::UnPossess();
	
	UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (bt)
	{
		bt->StopTree();
	}

}
