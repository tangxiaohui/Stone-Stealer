// Fill out your copyright notice in the Description page of Project Settings.

#include "UEProject05GameModeBase.h"
#include "Player/ACTPlayerController.h"
#include "UI/ACTHUD.h"
#include <ConstructorHelpers.h>
#include "Player/ACTCharacter.h"
#include "Monster/GemActor.h"
#include <Components/AudioComponent.h>
#include <Sound/AmbientSound.h>
#include <Engine/World.h>
#include "TimerManager.h"


void AUEProject05GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//GameBeginMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Begin.BGSound_Begin'"));
	//GamePlayMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Play.BGSound_Play'"));
	//GameOverMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Over.BGSound_Over'"));
	//FTimerHandle handl;
	//GetWorld()->GetTimerManager().SetTimer(handl, this, &AUEProject05GameModeBase::PlaySoundCallBack, 0.5f);
	
	BackgroundMusic = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());//SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Begin.BGSound_Begin'
	//BackgroundMusic->GetAudioComponent()->SetVolumeMultiplier(0.15f);
	BackgroundMusic->GetAudioComponent()->SetVolumeMultiplier(0);
	SetBGMusic(EBGState::EBS_Begin);
}

AUEProject05GameModeBase::AUEProject05GameModeBase() //Blueprint'/Game/ACTGame/Maps/NewBlueprint.NewBlueprint_C'
{
	static ConstructorHelpers::FClassFinder<AACTCharacter> CharacterClass(TEXT("Blueprint'/Game/ACTGame/Blueprints/Player/BP_ACTCharacter.BP_ACTCharacter_C'")); //Blueprint'/Game/ACTGame/Blueprints/Player/BP_ACTCharacter.BP_ACTCharacter_C'
	static ConstructorHelpers::FClassFinder<AACTHUD> HUDBPClass(TEXT("Blueprint'/Game/ACTGame/Blueprints/UI/BP_HUD.BP_HUD_C'"));
	
	static ConstructorHelpers::FObjectFinder<USoundBase> GameBeginMusicClass(TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Begin.BGSound_Begin'")); //Blueprint'/Game/ACTGame/Blueprints/Player/BP_ACTCharacter.BP_ACTCharacter_C'
	static ConstructorHelpers::FObjectFinder<USoundBase> GamePlayMusicClass(TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Play.BGSound_Play'")); //Blueprint'/Game/ACTGame/Blueprints/Player/BP_ACTCharacter.BP_ACTCharacter_C'
	static ConstructorHelpers::FObjectFinder<USoundBase> GameOverMusicClass(TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Over.BGSound_Over'")); //Blueprint'/Game/ACTGame/Blueprints/Player/BP_ACTCharacter.BP_ACTCharacter_C'

	GameBeginMusic = GameBeginMusicClass.Object;
	GamePlayMusic = GamePlayMusicClass.Object;
	GameOverMusic = GameOverMusicClass.Object;

	PlayerControllerClass = AACTPlayerController::StaticClass();
	//HUDClass = AACTHUD::StaticClass();
	if (HUDBPClass.Class)
	{
		HUDClass = HUDBPClass.Class;
	}
	DefaultPawnClass = CharacterClass.Class;
	
	
}

void AUEProject05GameModeBase::SetGem(AGemActor* Gem)
{
	this->Gem = Gem;
}

void AUEProject05GameModeBase::SetPlayer(class AACTCharacter* Player)
{
	this->Player = Player;
}

void AUEProject05GameModeBase::SetBGMusic(EBGState bgState)
{
	//if (BackgroundMusic)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("   ====have"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("   =====Null"));
	//}
	switch (bgState)
	{
	case EBGState::EBS_Begin:
		if (GameBeginMusic)
		{
			//GameBeginMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Begin.BGSound_Begin'"));
			BackgroundMusic->GetAudioComponent()->SetSound(GameBeginMusic);
			BackgroundMusic->Play();
		}
		break;
	case EBGState::EBS_Play:
		if (GamePlayMusic)
		{
			//GamePlayMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Play.BGSound_Play'"));
			BackgroundMusic->GetAudioComponent()->SetSound(GamePlayMusic);
			BackgroundMusic->Play();
		}
		break;
	case EBGState::EBS_Over:
		if (GameOverMusic)
		{
			//GameOverMusic = LoadObject<USoundBase>(this, TEXT("SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Over.BGSound_Over'"));
			BackgroundMusic->GetAudioComponent()->SetSound(GameOverMusic);
			BackgroundMusic->Play(84);
		}
		break;
	default:
		break;
	}
}

void AUEProject05GameModeBase::SetBGOn_Off(bool bTurnOn_Off)
{
	if (!BackgroundMusic)
	{
		return;
	}
	if (bTurnOn_Off)
	{
		BackgroundMusic->GetAudioComponent()->Play();
	}
	else
	{
		BackgroundMusic->GetAudioComponent()->Stop();
	}
}

void AUEProject05GameModeBase::PlaySoundCallBack()
{
	BackgroundMusic = GetWorld()->SpawnActor<AAmbientSound>(AAmbientSound::StaticClass());//SoundWave'/Game/ACTGame/Audios/BackgroundMusic/BGSound_Begin.BGSound_Begin'
	BackgroundMusic->GetAudioComponent()->SetVolumeMultiplier(0.15f);
	SetBGMusic(EBGState::EBS_Begin);
}

