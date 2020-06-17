// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_ShowTask.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_ShowTask : public UUserWidget
{
	GENERATED_BODY()
public:

		UFUNCTION()
		void OnPackUp();

protected:
	
		UFUNCTION(BlueprintImplementableEvent)
		void NotifyPlayAnimation();

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidght))
		class UTextBlock* Text;
	UPROPERTY()
		class AGemActor* Gems;
};
