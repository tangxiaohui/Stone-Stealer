// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ACTHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class UEPROJECT05_API AACTHUD : public AHUD
{
	GENERATED_BODY()
protected:

	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
private:
	UPROPERTY()
		class AACTCharacter* player;

	bool bIronsights;

public:
	void GetTaskUI();

	void ViewTask();

	void ShowDeadUI();

	class AACTCharacter* GetPlayer() { return player; }
	class UUserWidget_FindTask* GetTask() { return FindTaskWidget; }
	class UUserWidget_Task* GetViewTask() { return Task; }
	class UUserWidget_MainUI* GetMainUi() { return MainUI; }
	class UUserWidget_BeginPlay* GetBeginPlayUi() { return BeginUI; }
	class UUserWidget_Dead* GetDeadUi() { return DeadUI; }
protected:
	void DrawIronsights();
	UPROPERTY()
		class UUserWidget_FindTask* FindTaskWidget;
	UPROPERTY()
		class UUserWidget_Task* Task;
	UPROPERTY()
		class UUserWidget_MainUI* MainUI;
	UPROPERTY()
		class UUserWidget_GamUI* GemUI;
	UPROPERTY()
		class UUserWidget_BeginPlay* BeginUI;
	UPROPERTY()
		class UUserWidget_Dead* DeadUI;
	

	//Practice
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Widget")
	TSubclassOf<UUserWidget> HealthIndicatorClass;
	UPROPERTY()
	UUserWidget* HealthIndicator;
	//URadialForceComponent* RadialComp;


};
