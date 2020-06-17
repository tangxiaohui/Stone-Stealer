// Fill out your copyright notice in the Description page of Project Settings.

#include "GemActor.h"
#include "WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/Task/UserWidget_GamUI.h"
#include "UEProject05GameModeBase.h"
#include "ConstructorHelpers.h"

// Sets default values
AGemActor::AGemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widgets"));
	Widget->SetupAttachment(RootComponent);
	//static TSubclassOf<UUserWidget_GamUI> GemUIClass =<UUserWidget_GamUI>(this, TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_GamUI.UI_GamUI_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget_GamUI> GemUIClass(TEXT("WidgetBlueprint'/Game/ACTGame/Ui/Task/UI_GamUI.UI_GamUI_C'"));
	Widget->SetWidgetClass(GemUIClass.Class);
}

// Called when the game starts or when spawned
void AGemActor::BeginPlay()
{

	Super::BeginPlay();
	AUEProject05GameModeBase* GM=Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	GM->SetGem(this);

	Widget->SetDrawAtDesiredSize(true);
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
		
	Widget->SetHiddenInGame(true);

	

}

// Called every frame
void AGemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGemActor::OpenWidght()
{
	Widget->SetHiddenInGame(false);

}

