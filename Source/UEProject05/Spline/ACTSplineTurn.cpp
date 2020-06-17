// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTSplineTurn.h"
#include <Components/SplineComponent.h>

// Sets default values
AACTSplineTurn::AACTSplineTurn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SplineTrunComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));

	SetRootComponent(SceneComp);
	SplineTrunComp->SetupAttachment(SceneComp);
	
}

// Called when the game starts or when spawned
void AACTSplineTurn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACTSplineTurn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

