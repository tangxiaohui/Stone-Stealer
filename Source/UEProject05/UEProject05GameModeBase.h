// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UEProject05GameModeBase.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EBGState : uint8
{
	EBS_Begin,
	EBS_Play,
	EBS_Over,
};


UCLASS()
class UEPROJECT05_API AUEProject05GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AUEProject05GameModeBase();

	void SetGem(class AGemActor* Gem);

	void SetPlayer(class AACTCharacter* Player);
	UFUNCTION(BlueprintCallable)
	class AGemActor* GetGem() { return Gem; }
	class AACTCharacter* GetPlayer() { return Player; }

	void SetBGMusic(EBGState bgState);
	void SetBGOn_Off(bool bTurnOn_Off);

protected:
	UPROPERTY()
		class AGemActor* Gem;
	UPROPERTY()
		class AACTCharacter* Player;
	UPROPERTY()
	class AAmbientSound* BackgroundMusic;
	UPROPERTY()
		class USoundBase* GameBeginMusic;
	UPROPERTY()
		class USoundBase* GamePlayMusic;
	UPROPERTY()
		class USoundBase* GameOverMusic;


protected:
	void PlaySoundCallBack();
};
