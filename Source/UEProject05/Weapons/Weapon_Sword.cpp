// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Sword.h"
#include <ConstructorHelpers.h>
#include <Animation/AnimMontage.h>
#include <GameFramework/Character.h>
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimInstance.h>
#include <Engine/World.h>
#include <TimerManager.h>
#include "Player/ACTCharacter.h"
#include <Sound/SoundBase.h>

AWeapon_Sword::AWeapon_Sword()
{
	PrimaryActorTick.bCanEverTick = true;
	bSword = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontageResource01(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Attack/A_SW_Attack_01_Montage.A_SW_Attack_01_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontageResource02(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Attack/A_SW_Attack_02_Montage.A_SW_Attack_02_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontageResource03(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Attack/A_SW_Attack_03_Montage.A_SW_Attack_03_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontageResource04(TEXT("AnimMontage'/Game/ACTGame/Animations/Player/Sword/Attack/A_SW_HeavyAttack_Montage.A_SW_HeavyAttack_Montage'"));

	static ConstructorHelpers::FObjectFinder<USoundBase> damageSoundResource(TEXT("SoundWave'/Game/ACTGame/Audios/Attack/Attack_Hit.Attack_Hit'"));

	UAnimMontage* attackMontage01 = attackMontageResource01.Object;
	UAnimMontage* attackMontage02 = attackMontageResource02.Object;
	UAnimMontage* attackMontage03 = attackMontageResource03.Object;
	UAnimMontage* attackMontage04 = attackMontageResource04.Object;

	//if (!attackMontage01)
	//{
	//	return;
	//}

	AttackMontageArray.Add(attackMontage01);
	AttackMontageArray.Add(attackMontage02);
	AttackMontageArray.Add(attackMontage03);
	AttackMontageArray.Add(attackMontage04);

	DamageSound = damageSoundResource.Object;

	AttackMaxNum = AttackMontageArray.Num()-1;
	AttackNum = 0;
	MontagePlayTime = 0;
	bCanPlayMontage = true;
	bContinueCombo = false;
	bComboTrace = false;
	bBeginComboTrace = false;
	bCombo = false; 
	
}

void AWeapon_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("   bBeginComboTrace : %d "), bBeginComboTrace);
	if (bComboTrace)
	{
		if (BeginComboTrace > 0)
		{
			BeginComboTrace -= DeltaTime;
			//UE_LOG(LogTemp, Log, TEXT("   BeginComboTrace : %f "), BeginComboTrace);
		}
		else
		{
			bBeginComboTrace = true;

			if (EndComboTrace > 0)
			{
				EndComboTrace -= DeltaTime;
			}
			else
			{
				bComboTrace = false;
				bBeginComboTrace = false;
				bCanPlayMontage = true;
				AttackNum = 0;
			}
		}
	}

	//if (bComboTrace)
	//{
	//	if (BeginComboTrace>0)
	//	{
	//		BeginComboTrace -= DeltaTime;
	//	}
	//	else
	//	{
	//		bComboTrace = false;
	//		AttackNum = 0;
	//	}
	//}
	
}

void AWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

	WeaponComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Sword::OnComponentBeginOverlapEvent);
	//WeaponComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//if (AttackMontageArray[0])
	//{
	//	TimeBetweenAttacks= AttackMontageArray[0]->GetPlayLength()-0.8f;
	//}

}
void AWeapon_Sword::OnComponentBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogTemp, Log, TEXT("  sdf"));
	return;

	if (bCanPlayMontage||OtherActor==WeaponOwner||WeaponOwner==nullptr)
	{
		return;
	}
	
	FDamageEvent damageEvent;
	OtherActor->TakeDamage(3, damageEvent,WeaponOwner->GetController(),this);
}

void AWeapon_Sword::TakeAttack()
{
	Super::TakeAttack();
	//UE_LOG(LogTemp, Log, TEXT("   asdf : %d"),AttackMaxNum);

	//if (bBeginComboTrace)
	//{
	//	AttackNum++;
	//	bComboTrace = false;
	//	bCanPlayMontage = true;
	//	bBeginComboTrace = false;

	//	PlayAttactMontage();
	//}

	//if (bCanPlayMontage)
	//{
	//	PlayAttactMontage();
	//}
	//UE_LOG(LogTemp, Log, TEXT(" Take Attack  "));

	//bCombo = true;
	//LastAttackTime = GetWorld()->TimeSeconds;
	PlayAttackMontage1();

}

void AWeapon_Sword::BeginAttack()
{

	if (LastAttackTime+TimeBetweenAttacks-GetWorld()->TimeSeconds>0)
	{
		return;
	}

	
	
	//UE_LOG(LogTemp, Log, TEXT(" Begin Attack  "));
	//GetWorld()->GetTimerManager().SetTimer(attackTimeBetweenHandle, this, &AWeapon_Sword::TakeAttack,TimeBetweenAttacks);
	//if (LastAttackTime+TimeBetweenAttacks-GetWorld()->TimeSeconds>-0.5f)
	//{
	//	return;
	//}
	
	TakeAttack();
	//PlayAttackMontage1();
}

void AWeapon_Sword::StopAttack()
{
	Super::StopAttack();
	float resetTime = FMath::Max(LastAttackTime - GetWorld()->TimeSeconds + TimeBetweenAttacks,0.0f);
	GetWorld()->GetTimerManager().SetTimer(resetComboHandle, this, &AWeapon_Sword::ResetComboCallBack, resetTime + .5f);
}

void AWeapon_Sword::PlayAttactMontage()
{
	if (!WeaponOwner&&AttackNum==-1)
	{
		return;
	}
	if (AttackNum>AttackMaxNum)
	{
		AttackNum = 0;
	}

	MontagePlayTime=WeaponOwner->GetMesh()->GetAnimInstance()->Montage_Play(AttackMontageArray[AttackNum]);
	bCanPlayMontage = false;
	//FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &AWeapon_Sword::CanPlayMontageCallBack, MontagePlayTime);

	bComboTrace = true;
	BeginComboTrace = (MontagePlayTime - 0.8f)<0?0:(MontagePlayTime - 0.8f);
	EndComboTrace = ((MontagePlayTime - 0.5f)<0?(MontagePlayTime - 0.5f):0) + 1;
	
	//AACTCharacter* player = Cast<AACTCharacter>(WeaponOwner);
	//if (player)
	//{
	//	player->SetPlayerState(EPlayerState::EPS_DND);
	//}
	//UE_LOG(LogTemp, Log, TEXT("  MontagePlayTime: %f"), MontagePlayTime);
	//UE_LOG(LogTemp, Log, TEXT("  BeginComboTrace: %f"),BeginComboTrace);
	//UE_LOG(LogTemp, Log, TEXT("  EndComboTrace: %f"),EndComboTrace);
}

void AWeapon_Sword::PlayAttackMontage1()
{
	if (!WeaponOwner)
	{
		return;
	}

	bCombo = true;
	LastAttackTime = GetWorld()->TimeSeconds;

	WeaponOwner->GetMesh()->GetAnimInstance()->Montage_Play(AttackMontageArray[AttackNum]);
	AttackNum++;
	if (AttackNum > AttackMaxNum) AttackNum = 0;
	//TimeBetweenAttacks = AttackMontageArray[AttackNum]->GetPlayLength()-0.8f;
	UE_LOG(LogTemp, Log, TEXT("  GetPlayLength: %d"),AttackNum);
	if (AttackNum == 0)
	{
		TimeBetweenAttacks = AttackMontageArray[AttackMaxNum]->GetPlayLength() - 0.8f;
	}
	else
	{
		TimeBetweenAttacks = AttackMontageArray[AttackNum - 1]->GetPlayLength() - 0.8f;
	}
	if (!bCombo)
	{
		TimeBetweenAttacks = 0;
	}
	
	GetWorld()->GetTimerManager().SetTimer(attackTimeBetweenHandle,this,&AWeapon_Sword::PlayAttackMontage1,TimeBetweenAttacks);
	
	GetWorld()->GetTimerManager().ClearTimer(resetComboHandle);
}

//void AWeapon_Sword::CanPlayMontageCallBack()
//{
	//if (bContinueCombo)
	//{
	//	if (AttackNum<AttackMaxNum)
	//	{
	//		AttackNum++;
	//	}
	//	else
	//	{
	//		AttackNum = 0;
	//	}
	//}
	//else
	//{
	//	FTimerHandle handle;
	//	GetWorld()->GetTimerManager().SetTimer(handle, this, &AWeapon_Sword::ResetComboCallBack, 0.5f);
	//}
	//if (BeginComboTrace>0||EndComboTrace>0)
	//{
	//	return;
	//}
	//bCanPlayMontage = true;
	//bComboTrace = true;
	//BeginComboTrace = 0.8f;

	//AACTCharacter* player = Cast<AACTCharacter>(WeaponOwner);
	//if (player)
	//{
	//	player->SetPlayerState(EPlayerState::EPS_Normal);
	//}
//}

void AWeapon_Sword::ResetComboCallBack()
{
	AttackNum = 0;
	bCombo = false;
	//GetWorld()->GetTimerManager().ClearTimer(attackTimeBetweenHandle);
}
