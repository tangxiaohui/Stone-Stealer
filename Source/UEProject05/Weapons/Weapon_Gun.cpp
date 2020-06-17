// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "ShellActor.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Player/ACTCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <ConstructorHelpers.h>
#include "DrawDebugHelpers.h"
#include "WeaponBasic.h"
#include "IConsoleManager.h"
#include "Particles/ParticleSystemComponent.h"


static int32 DebugWeaponValue = 0;
FAutoConsoleVariableRef CVFDebugWeapon(TEXT("Jing.DebugWeapon"),DebugWeaponValue,TEXT("Weapon Debug Line"),ECVF_Cheat);

AWeapon_Gun::AWeapon_Gun()
{
	bSword = false;


	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundClass(TEXT("SoundWave'/Game/ACTGame/Audios/Attack/Gun_AK.Gun_AK'"));
	FireSound = FireSoundClass.Object;

	MuzzleSocketName = TEXT("Muzzle");
	BulletSpread = 0.0f;
}

void AWeapon_Gun::TakeAttack()
{
	Super::TakeAttack();
	if (!WeaponOwner)
	{
		return;
	}

	//AACTCharacter* player = Cast<AACTCharacter>(WeaponOwner);
	//
	//if (player)
	//{
	//	PlayerAttack();
	//}
	//else
	//{
	//	MonsterAttack();
	//}

	PlayerAttack();


	WeaponEffect();

}

void AWeapon_Gun::WeaponEffect()
{

	//Muzzle
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponComp, MuzzleSocketName);
	}

	//trajectory
	if (TracerEffect)
	{
		UParticleSystemComponent* tracerComp= UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TracerEffect,MuzzlePos);
		if (tracerComp)
		{
			tracerComp->SetVectorParameter("BeamEnd", TracerEnd);
		}
	}

	//wav
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.15f);
	}

}

//void AWeapon_Gun::MonsterAttack()
//{
//	MuzzlePos = WeaponComp->GetSocketLocation(TEXT("Muzzle"));
//
//	Shells = GetWorld()->SpawnActor<AShellActor>(LoadClass<AShellActor>(this, TEXT("Blueprint'/Game/ACTGame/Blueprints/Weapon/BP_Shell.BP_Shell_C'")), MuzzlePos, FRotator::ZeroRotator);
//	Shells->SetShellOwner(WeaponOwner);
//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.15f);
//
//	if (!WeaponOwner)
//	{
//		return;
//	}
//	FVector PlayerPos;
//	AController* MC = Cast<AController>(WeaponOwner->GetController());
//	AAIController* Cont = Cast<AAIController>(MC);
//	if (Cont)
//	{
//		UObject* obj = Cont->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
//		AActor* act = Cast<AActor>(obj);
//		if (act)
//		{
//			PlayerPos = act->GetActorLocation();
//		}
//	}
//	FVector dir = (PlayerPos + FVector(0, 0, 90)) - MuzzlePos;
//	dir.Normalize();
//
//	UStaticMeshComponent* Mash = Cast<UStaticMeshComponent>(Shells->GetMesh());
//	Mash->SetSimulatePhysics(true);
//	Mash->AddImpulse(dir * 100);
//	
//	Shells->SetLifeSpan(3);
//}


void AWeapon_Gun::PlayerAttack()
{
	////UE_LOG(LogTemp, Log, TEXT("   PlayerAttack"));
	//APlayerController* pc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	//if (!pc)
	//{
	//	return;
	//}
	//int32 sizeX;
	//int32 sizeY;
	//pc->GetViewportSize(sizeX, sizeY);
	FVector worldPos;
	FVector worldDir;
	//pc->DeprojectScreenPositionToWorld(sizeX / 2, sizeY / 2, worldPos, worldDir);

	FVector ownerPos;
	FRotator ownerRot;
	WeaponOwner->GetActorEyesViewPoint(ownerPos, ownerRot);

	worldDir = ownerRot.Vector();
	worldPos = ownerPos;

	MuzzlePos = WeaponComp->GetSocketLocation(TEXT("Muzzle"));
	FVector shellDir= worldDir;

	//Random
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	shellDir = FMath::VRandCone(shellDir, HalfRad, HalfRad);
	shellDir.Normalize();

	TracerEnd = worldPos + shellDir * 5000;

	FHitResult screenHit;
	FCollisionQueryParams wparam;
	wparam.AddIgnoredActor(this);
	wparam.AddIgnoredActor(WeaponOwner);
	if (GetWorld()->LineTraceSingleByChannel(screenHit, worldPos, worldPos + shellDir * 5000, ECC_Camera,wparam))
	{
		//shellDir = screenHit.Location - MuzzlePos;
		//shellDir = screenHit.ImpactPoint - MuzzlePos;
		//shellDir.Normalize();
		TracerEnd = screenHit.ImpactPoint;
		//UE_LOG(LogTemp, Log, TEXT(" hit "));
	}

	if (!ShellClass)
	{
		return;
	}

	shellDir = TracerEnd - MuzzlePos;
	shellDir.Normalize();

	Shells = GetWorld()->SpawnActor<AShellActor>(ShellClass, MuzzlePos, shellDir.Rotation());

	
	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Shells->GetMesh());
	Shells->SetShellOwner(WeaponOwner);
	Shells->SetOwnerWeapon(this);
	mesh->SetSimulatePhysics(true);
	mesh->AddImpulse(shellDir * 3000);

	//DrawDebugLine(GetWorld(), MuzzlePos, MuzzlePos + shellDir * 1000, FColor::Red, false, 1.0f, 0, 1.0f);
	




	//Console Variable Test
	if (DebugWeaponValue>0)
	{
		DrawDebugLine(GetWorld(), WeaponComp->GetSocketLocation(MuzzleSocketName), worldPos + worldDir * 1000, FColor::Red,false,1.0f,0,1.0f);
		UE_LOG(LogTemp, Log, TEXT("Weapon Debug Line"));
	}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("DebugWeaponValue: %d"),DebugWeaponValue);
	//}

	//Camera Shake Test
	//APawn* player = Cast<APawn>(WeaponOwner);
	//if (player)
	//{
	//	APlayerController* pc = Cast<APlayerController>(player->GetController());
	//	if (pc&&CameraShakeTest)
	//	{
	//		pc->ClientPlayCameraShake(CameraShakeTest);
	//	}
	//}

	LastAttackTime = GetWorld()->TimeSeconds;
}

void AWeapon_Gun::TestFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Weapon Test Function"));
}
