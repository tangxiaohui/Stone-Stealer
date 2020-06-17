// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_MainUI.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ACTCharacter.h"
#include "ACTHUD.h"
#include "GameFramework/PlayerController.h"
#include "Task/UserWidget_Task.h"
#include "Task/UserWidget_ShowTask.h"
#include "VerticalBox.h"

void UUserWidget_MainUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* HUD = Cast<AACTHUD>(PC->GetHUD());
	if (!HUD)
	{
		return;
	}
	//绑定通知	先获得玩家-----在得到玩家里面的背包组件（多播源）
	HUD->GetViewTask()->NotifyCall.BindUObject(this, &UUserWidget_MainUI::UpdateTaskVerticalBox);

}

void UUserWidget_MainUI::UpdateTaskVerticalBox()
{
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	static TSubclassOf<UUserWidget_ShowTask> PackageItemClass = LoadClass<UUserWidget_ShowTask>(nullptr, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_ShowTask.UI_ShowTask_C'"));
		//创建
	TaskItem = CreateWidget<UUserWidget_ShowTask>(PC, PackageItemClass);
		if (GetTaskVerticalBox())
		{
			//在吧PageitemWidght添加到武器列表里
			GetTaskVerticalBox()->AddChild(TaskItem);
		}	
}
