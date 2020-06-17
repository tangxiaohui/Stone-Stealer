// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainUI.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_MainUI : public UUserWidget
{
	GENERATED_BODY()
public:
	class UUserWidget_ShowTask* GetShowTask() { return TaskItem; }

	UFUNCTION(BlueprintImplementableEvent)
	void SetProgressBarPercent(float Value);
protected:

	virtual void NativeOnInitialized() override;


		UFUNCTION(BlueprintImplementableEvent)
		class UVerticalBox* GetTaskVerticalBox();

		void UpdateTaskVerticalBox();
protected:
		UPROPERTY()
			class AACTCharacter* CurrentPlayer;
		UPROPERTY()
		class UUserWidget_ShowTask* TaskItem;
};
