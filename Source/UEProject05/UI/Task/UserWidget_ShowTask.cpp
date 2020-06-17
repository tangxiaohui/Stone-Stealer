// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_ShowTask.h"
#include "Button.h"
#include "TextBlock.h"
#include "UEProject05GameModeBase.h"
#include "Monster/GemActor.h"
#include "UnrealString.h"

void UUserWidget_ShowTask::OnPackUp()
{
	
	NotifyPlayAnimation();
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	Gems = Cast<AGemActor>(GM->GetGem());
	if (!Gems)
	{

		Text->SetText(FText::FromString(TEXT("已完成")));
		UE_LOG(LogTemp, Log, TEXT("tab"));
	}


}

void UUserWidget_ShowTask::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	/*if (!Gems)
	{
		return;
	}*/
	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_4")));
}
