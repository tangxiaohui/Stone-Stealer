// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTHUD.h"
#include "Player/ACTCharacter.h"
#include <Engine/Canvas.h>
#include "Kismet/GameplayStatics.h"
#include "Task/UserWidget_FindTask.h"
#include "GameFramework/PlayerController.h"
#include "Task/UserWidget_Task.h"
#include "UserWidget_MainUI.h"
#include "Task/UserWidget_ShowTask.h"
#include "Task/UserWidget_GamUI.h"
#include "UserWidget_BeginPlay.h"
#include "UserWidget_Dead.h"

void AACTHUD::DrawHUD()
{
	Super::DrawHUD();

	player = Cast<AACTCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!player)
	{
		return;
	}



	DrawIronsights();
}

void AACTHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* Pc = UGameplayStatics::GetPlayerController(this, 0);
	player = Cast<AACTCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!Task)
	{
		//如果之前没有创建背包控件，则创建背包控件
		TSubclassOf<UUserWidget_Task> TaskUIClass = LoadClass<UUserWidget_Task>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_Tasks.UI_Tasks_C'"));
		//把加载的背包资源，装载到声明好的背包控件指针里，（给指针赋值）
		Task = Cast<UUserWidget_Task>(CreateWidget(Pc, TaskUIClass));
	}
	TSubclassOf<UUserWidget_MainUI> MainUIClass = LoadClass<UUserWidget_MainUI>(nullptr, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/UI_MainUI.UI_MainUI_C'"));
	//构建控件实例，第一个参数一般是playercontroller
	MainUI = CreateWidget<UUserWidget_MainUI>(Pc, MainUIClass);
	if (MainUI)
	{
		MainUI->AddToViewport();
	}
	
	////The Begin UI
	////如果之前没有创建背包控件，则创建背包控件
	//TSubclassOf<UUserWidget_BeginPlay> BeginUIClass = LoadClass<UUserWidget_BeginPlay>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/UI_PlayBegin.UI_PlayBegin_C'"));
	////把加载的背包资源，装载到声明好的背包控件指针里，（给指针赋值）
	//BeginUI = Cast<UUserWidget_BeginPlay>(CreateWidget(Pc, BeginUIClass));
	//if (BeginUI)
	//{
	//	BeginUI->AddToViewport();
	//	FInputModeUIOnly Mode;
	//	Pc->SetInputMode(Mode);
	//}

	//if (Pc)
	//{
	//	Pc->bShowMouseCursor = true;
	//}

	//if (HealthIndicatorClass)
	//{
	//	
	//}
	if (HealthIndicatorClass)
	{
		HealthIndicator = CreateWidget<UUserWidget>(Pc, HealthIndicatorClass);
		HealthIndicator->AddToViewport();
	}
}

void AACTHUD::GetTaskUI()
{
	APlayerController* Pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!FindTaskWidget)
	{
		//如果之前没有创建背包控件，则创建背包控件
		TSubclassOf<UUserWidget_FindTask> FindTaskclass = LoadClass<UUserWidget_FindTask>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_FindTask.UI_FindTask_C'"));
		//把加载的背包资源，装载到声明好的背包控件指针里，（给指针赋值）
		FindTaskWidget = Cast<UUserWidget_FindTask>(CreateWidget(Pc, FindTaskclass));
		
	}
	if (!FindTaskWidget)
	{
		return;
	}
	if (!FindTaskWidget->IsInViewport())
	{
		FindTaskWidget->AddToViewport();
	}
	
}

void AACTHUD::ViewTask()
{
	////先调取控制器，
	//APlayerController* Pc = UGameplayStatics::GetPlayerController(this, 0);
	//if (!Task)
	//{
	//	//如果之前没有创建背包控件，则创建背包控件
	//	TSubclassOf<UUserWidget_Task> TaskUIClass = LoadClass<UUserWidget_Task>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_Tasks.UI_Tasks_C'"));
	//	//把加载的背包资源，装载到声明好的背包控件指针里，（给指针赋值）
	//	Task = Cast<UUserWidget_Task>(CreateWidget(Pc, TaskUIClass));
	//}
	APlayerController* Pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!Task)
	{
		return;
	}
	//背包开关
	if (!Task->IsInViewport())
	{
		Task->AddToViewport();
	}
	//显示鼠标开关
	if (Pc)
	{
		Pc->bShowMouseCursor = Task->IsInViewport();
	}
}



void AACTHUD::ShowDeadUI()
{
	UE_LOG(LogTemp, Log, TEXT("  asdf"));
	APlayerController* Pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!DeadUI)
	{
		//如果之前没有创建背包控件，则创建背包控件
		TSubclassOf<UUserWidget_Dead> DeadUIclass = LoadClass<UUserWidget_Dead>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/BP_Dead.BP_Dead_C'"));
		//把加载的背包资源，装载到声明好的背包控件指针里，（给指针赋值）
		DeadUI = Cast<UUserWidget_Dead>(CreateWidget(Pc, DeadUIclass));

	}
	if (!DeadUI)
	{
		return;
	}
	if (!DeadUI->IsInViewport())
	{
		DeadUI->AddToViewport();
	}
	if (Pc)
	{
		Pc->bShowMouseCursor = true;
	}

}

void AACTHUD::DrawIronsights()
{
	if (!player->IsHoldWeapon())
	{
		return;
	}
	if (player->IsHoldSword())
	{
		return;
	}

	float canvasX = Canvas->ClipX / 2;
	float canvasY = Canvas->ClipY / 2;

	float aimLength = 20;
	float aimDistance = 0;
	if (player->GetIronsightsMove() >= 3 && player->GetIronsightsMove() <= 19)
	{
		aimDistance = player->GetIronsightsMove();
	}
	else if (player->GetIronsightsMove()<3)
	{
		aimDistance = 3;
	}
	else if (player->GetIronsightsMove()>19)
	{
		aimDistance = 19;
	}

	//上
	DrawLine(canvasX, canvasY + aimDistance, canvasX, canvasY + aimDistance + aimLength, FColor::Green);
	//下
	DrawLine(canvasX, canvasY - aimDistance, canvasX, canvasY - aimDistance - aimLength, FColor::Green);
	//左
	DrawLine(canvasX + aimDistance, canvasY, canvasX + aimDistance + aimLength, canvasY, FColor::Green);
	//右
	DrawLine(canvasX - aimDistance, canvasY, canvasX - aimDistance - aimLength, canvasY, FColor::Green);
}
