// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_Dead.h"
#include "Button.h"
#include "Player/ACTCharacter.h"
#include "GameFramework/PlayerController.h"
#include "ACTHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UEProject05GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UUserWidget_Dead::GoOnGame()
{
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(UGameplayStatics::GetGameMode(this));
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	AACTCharacter* player = Cast<AACTCharacter>(hud->GetPlayer());
	//GM->ResetLevel();
	FInputModeGameOnly Ingo;
	PC->SetInputMode(Ingo);
	UKismetSystemLibrary::ExecuteConsoleCommand(player,TEXT("RestartLevel"));
	hud->GetDeadUi()->RemoveFromParent();
}

void UUserWidget_Dead::GameEnd()
{
	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	AACTHUD* hud = Cast<AACTHUD>(PC->GetHUD());
	AACTCharacter* player = Cast<AACTCharacter>(hud->GetPlayer());
	//TEnumAsByte<EQuitPreference::Quit> QuitEnum;
	UKismetSystemLibrary::QuitGame(player, PC, EQuitPreference::Quit, false);
}

void UUserWidget_Dead::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UButton* Btu = Cast<UButton>(GetWidgetFromName(TEXT("Button_150")));
	if (Btu)
	{
		Btu->OnClicked.AddDynamic(this, &UUserWidget_Dead::GoOnGame);
	}
	UButton* Btd = Cast<UButton>(GetWidgetFromName(TEXT("Button_151")));
	if (Btd)
	{
		Btd->OnClicked.AddDynamic(this, &UUserWidget_Dead::GameEnd);
	}
}
