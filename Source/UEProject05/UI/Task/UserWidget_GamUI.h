// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_GamUI.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_GamUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidght))
		class UTextBlock* TextPos;
	
};
