// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_BeginPlay.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_BeginPlay : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
		void OnGameBegin();
	UFUNCTION()
		void OnGameEnd();

	virtual void NativeOnInitialized() override;
};
