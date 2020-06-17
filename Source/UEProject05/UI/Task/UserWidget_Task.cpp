// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_Task.h"
#include "Button.h"
#include "GameFramework/PlayerController.h"
#include "UI/ACTHUD.h"
#include "Player/ACTCharacter.h"
#include "Monster/NPC_Character.h"
#include "UserWidget_FindTask.h"
#include "UEProject05GameModeBase.h"
#include "Monster/GemActor.h"


void UUserWidget_Task::OnReceive()
{
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	AACTCharacter* player = Cast<AACTCharacter>(PC->GetCharacter());
	ANPC_Character* NPC = player->GetTaskNPC();
	if (!NPC)
	{
		return;
	}
	NPC->DeleteHind();
	if (PC)
	{
		if (!hud)
		{
			return;
		}
		PC->bShowMouseCursor = false;
		player->ReceiveTask();
		player->NoFindTask();

		if (!hud->GetViewTask())
		{
			return;
		}
	
		hud->GetTask()->RemoveFromParent();
		hud->GetViewTask()->RemoveFromParent();
	
	}
	if (NotifyCall.IsBound())
	{
		NotifyCall.Execute();
	}
	AGemActor* Gems = Cast<AGemActor>(GM->GetGem());
	Gems->OpenWidght();
	
}

void UUserWidget_Task::OnCancel()
{
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	if (PC)
	{
		if (!hud)
		{
			return;
		}
		PC->bShowMouseCursor = false;
		if (!hud->GetViewTask())
		{
			return;
		}
		hud->GetViewTask()->RemoveFromParent();
	}
}

void UUserWidget_Task::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UButton* Btn = Cast<UButton>(GetWidgetFromName(TEXT("Button_62")));
	if (Btn)
	{
		Btn->OnClicked.AddDynamic(this, &UUserWidget_Task::OnReceive);
	}
	UButton* MaxBtn = Cast<UButton>(GetWidgetFromName(TEXT("Button_63")));
	if (MaxBtn)
	{
		MaxBtn->OnClicked.AddDynamic(this, &UUserWidget_Task::OnCancel);
	}
}
