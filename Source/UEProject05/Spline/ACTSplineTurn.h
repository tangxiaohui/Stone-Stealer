// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACTSplineTurn.generated.h"

UCLASS()
class UEPROJECT05_API AACTSplineTurn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACTSplineTurn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* SplineTrunComp;


public:
	class USplineComponent* GetSplineTurnComp() const { return SplineTrunComp; }

};
