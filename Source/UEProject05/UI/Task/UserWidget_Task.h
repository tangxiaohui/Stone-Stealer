// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Task.generated.h"
DECLARE_DELEGATE(Del_Call)
/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_Task : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
		void OnReceive();
	UFUNCTION()
		void OnCancel();

	virtual void NativeOnInitialized() override;
public:
	Del_Call NotifyCall;
};
