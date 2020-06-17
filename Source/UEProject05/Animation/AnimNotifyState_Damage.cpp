// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_Damage.h"
#include <Components/SkeletalMeshComponent.h>
#include "Player/ACTCharacter.h"
#include "Weapons/WeaponBasic.h"
#include <DrawDebugHelpers.h>

#define SOLTNAME "SwordSlot_0"
#define CONSNAME(num) (SOLTNAME)##(#num)




void UAnimNotifyState_Damage::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AACTCharacter* player = Cast<AACTCharacter>(MeshComp->GetOwner());
	if (!player)
	{
		return;
	}
	if (!player->IsHoldSword())
	{
		return;
	}
	for (int32 i=1;i<=6;i++)
	{
		SlotPos.Add(player->GetHoldingWeapon()->GetWeaponComp()->GetSocketLocation(*FString::Printf(TEXT("SwordSlot_0%d"), 1)));
	}
}

void UAnimNotifyState_Damage::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);


	//UE_LOG(LogTemp, Log, TEXT("  asdf"));
	AACTCharacter* player = Cast<AACTCharacter>(MeshComp->GetOwner());
	if (!player)
	{
		return;
	}
	if (!player->IsHoldSword())
	{
		return;
	}
	
	//for (int32 i = 1; i <= 6; i++)
	//{
	//	DrawDebugLine(MeshComp->GetWorld(), SlotPos[i-1], player->GetHoldingWeapon()->GetWeaponComp()->GetSocketLocation(*FString::Printf(TEXT("SwordSlot_0%d"), i)), FColor::Red, false, 1);
	//}
	
	//DrawDebugLine(MeshComp->GetWorld(), SlotPos_01, player->GetActorLocation(), FColor::Red);
	//DrawDebugLine(MeshComp->GetWorld(), player->GetActorLocation(), player->GetActorLocation() + FVector::UpVector * 200, FColor::Red);
	//DrawDebugLine(MeshComp->GetWorld(), player->GetActorLocation(), SlotPos_01, FColor::Red);

	FCollisionQueryParams param;
	param.AddIgnoredActor(player);

	for (int32 i = 1; i <= 6; i++)
	{
		FHitResult damageHit;
		if (MeshComp->GetWorld()->LineTraceSingleByChannel(damageHit, SlotPos[i - 1],
			player->GetHoldingWeapon()->GetWeaponComp()->GetSocketLocation(*FString::Printf(TEXT("SwordSlot_0%d"), i)), ECC_Camera, param))
		{
			if (HitActorArray.Contains(damageHit.GetActor()))
			{
				continue;
			}
			HitActorArray.Add(damageHit.GetActor());
			//damageHit.GetActor()->TakeDamage(3, damageEvent, player->GetController(), player->GetHoldingWeapon());
			player->GetHoldingWeapon()->SetDamage(damageHit.GetActor());
		}

	}



	for (int32 i = 1; i <= 6; i++)
	{
		SlotPos[i-1] = player->GetHoldingWeapon()->GetWeaponComp()->GetSocketLocation(*FString::Printf(TEXT("SwordSlot_0%d"),i));
	}


	//UE_LOG(LogTemp, Log, TEXT("   %s"),*slot_01.ToString());


}

void UAnimNotifyState_Damage::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	HitActorArray.Empty(6);
}
