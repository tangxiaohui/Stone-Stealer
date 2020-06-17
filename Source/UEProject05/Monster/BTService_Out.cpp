// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Out.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NPC_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ACTHUD.h"
#include "GameFramework/PlayerController.h"
#include "UI/Task/UserWidget_FindTask.h"
#include "UserWidget.h"
#include "Player/ACTCharacter.h"
#include "UI/Task/UserWidget_Task.h"

void UBTService_Out::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ANPC_Character* NPC = Cast<ANPC_Character>(OwnerComp.GetAIOwner()->GetPawn());
//	APawn* Player = OwnerComp.GetAIOwner()->GetPawn();
	FVector Pos;
	UObject* Obj=OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
	AActor* act = Cast<AActor>(Obj);
	Player = Cast<AACTCharacter>(act);
	if (act)
	{
		Pos = act->GetActorLocation();

	}
	
	if ((Pos - NPC->GetActorLocation()).Size2D() > ArceValues)
	{
		APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(NPC, 0));
		AACTHUD* HUD = Cast<AACTHUD>(PC->GetHUD());
		if (PC)
		{
			
			if (!HUD)
			{
				return;
			}
			if (HUD->GetTask()->IsInViewport())
			{
				
				HUD->GetTask()->RemoveFromParent();
				Player->NoFindTask();
			}
			PC->bShowMouseCursor = false;
			if (!HUD->GetViewTask())
			{
				return;
			}
			HUD->GetViewTask()->RemoveFromParent();
			
			
			
		}
		
		//OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
	}
	

		
}


