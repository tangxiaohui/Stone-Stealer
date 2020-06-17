// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_GamUI.h"
#include "TextBlock.h"
#include "UEProject05GameModeBase.h"
#include "Player/ACTCharacter.h"
#include "Monster/GemActor.h"

void UUserWidget_GamUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TextPos = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_0")));
}

void UUserWidget_GamUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	AACTCharacter* Player = Cast<AACTCharacter>(GM->GetPlayer());

	AGemActor* Gem = Cast<AGemActor>(GM->GetGem());
	if (!Gem||!Player)
	{
		return;
	}
	int32 Disition; 
	FVector Lenthl;
	Lenthl = Player->GetActorLocation() - Gem->GetActorLocation();
	Disition = Lenthl.Size()-70;
	//UE_LOG(LogTemp, Log, TEXT("%f"), Disition);
	//FText Dis=Disition.
	TextPos->SetText(FText::FromString(FString::Printf(TEXT("%d"), Disition)));
}