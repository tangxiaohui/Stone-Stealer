// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Dead.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT05_API UUserWidget_Dead : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
		void GoOnGame();
	UFUNCTION()
		void GameEnd();



	virtual void NativeOnInitialized() override;
};
