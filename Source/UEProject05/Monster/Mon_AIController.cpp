// Fill out your copyright notice in the Description page of Project Settings.

#include "Mon_AIController.h"
#include "Monster_Character.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

void AMon_AIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
	AMonster_Character* Mon = Cast<AMonster_Character>(InPawn);
	if (!Mon)
	{
		return;
	}
	RunBehaviorTree(Mon->GetBehaviorTree());
	if (Blackboard)
	{
		//UBlackboardComponent
		Blackboard->SetValueAsVector(TEXT("StartPos"), InPawn->GetActorLocation());
	}
	FVector Dir(1,0,0);
	Dir = Dir.RotateAngleAxis(FMath::FRandRange(0, 360), FVector::UpVector);
	Blackboard->SetValueAsVector(TEXT("TargetPos"), InPawn->GetActorLocation() + Dir*FMath::FRandRange(400, 800));
	UE_LOG(LogTemp, Log, TEXT("ppppppp"));
}

void AMon_AIController::UnPossess()
{
	Super::UnPossess();
	UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (bt)
	{
		bt->StopTree();
	}
	
}


