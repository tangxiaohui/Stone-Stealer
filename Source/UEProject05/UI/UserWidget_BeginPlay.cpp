// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_BeginPlay.h"
#include "Button.h"
#include "UEProject05GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "ACTHUD.h"
#include "Player/ACTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnumAsByte.h"
#include <Kismet/GameplayStatics.h>
void UUserWidget_BeginPlay::OnGameBegin()
{
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	AACTCharacter* player = Cast<AACTCharacter>(hud->GetPlayer());
	hud->GetBeginPlayUi()->RemoveFromParent();

	//PC->Possess(player);
	FInputModeGameOnly Into;
	PC->SetInputMode(Into);
	if (PC)
	{
		PC->bShowMouseCursor = false;
	}

	//AUEProject05GameModeBase* gm = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->SetBGMusic(EBGState::EBS_Play);
	}

}

void UUserWidget_BeginPlay::OnGameEnd()
{
	//KismetSystemLibary
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	AACTCharacter* player = Cast<AACTCharacter>(hud->GetPlayer());
	//TEnumAsByte<EQuitPreference::Quit> QuitEnum;
	UKismetSystemLibrary::QuitGame(player,PC, EQuitPreference::Quit,false);
}

void UUserWidget_BeginPlay::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UButton* Btu = Cast<UButton>(GetWidgetFromName(TEXT("Button_1")));
	if (Btu)
	{
		Btu->OnClicked.AddDynamic(this, &UUserWidget_BeginPlay::OnGameBegin);
	}
	UButton* Btd= Cast<UButton>(GetWidgetFromName(TEXT("Button_2")));
	if (Btd)
	{
		Btd->OnClicked.AddDynamic(this, &UUserWidget_BeginPlay::OnGameEnd);
	}
}
