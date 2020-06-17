// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GemActor.generated.h"

UCLASS()
class UEPROJECT05_API AGemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenWidght();

protected:

	
	UPROPERTY()
		class UWidgetComponent* Widget;
	UPROPERTY()
		class USceneComponent* Root;
	/*UPROPERTY()
		class TSubclassOf<UUserWidget_GamUI> GemUIClass;*/
};
