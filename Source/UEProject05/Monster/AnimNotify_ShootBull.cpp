// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_ShootBull.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/Weapon_Gun.h"
#include "Monster_Character.h"

#include "GameFramework/Controller.h"

void UAnimNotify_ShootBull::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	APawn* _Pawn = Cast<APawn>(MeshComp->GetOwner());
	AMonster_Character* cha = Cast<AMonster_Character>(_Pawn);
	AController* Controller = _Pawn ? _Pawn->GetController() : nullptr;

	if (!MeshComp->GetOwner() || !Controller)
	{
		return;
	}
	AWeapon_Gun* Gun = Cast<AWeapon_Gun>(cha->GetHoldGun());
	if (!Gun)
	{
		return;
	}
	Gun->TakeAttack();

}
