// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBasic.h"
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CoreNet.h"
#include "UnrealNetwork.h"

// Sets default values
AWeaponBasic::AWeaponBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(WeaponComp);

	WeaponComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponComp->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	WeaponComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	RotaOfAttack = 300;
	FirstDelay = 0;

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void AWeaponBasic::BeginPlay()
{
	Super::BeginPlay();
	WeaponComp->SetSimulatePhysics(true);

	TimeBetweenAttacks = 60 / RotaOfAttack;
}

// Called every frame
void AWeaponBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBasic::SetWeaponOwner(class ACharacter* player)
{
	if (!player)
	{
		WeaponOwner = nullptr;
		WeaponComp->SetSimulatePhysics(true);
		WeaponComp->AddImpulse(GetActorForwardVector() * 500);
		WeaponComp->AddImpulse(FVector::UpVector * 800);
		return;
	}
	//
	SetOwner(player);
	WeaponOwner = player;
	WeaponComp->SetSimulatePhysics(false);
	
}

void AWeaponBasic::SetWeaponAttach(FName name)
{
	if (!WeaponOwner)
	{
		return;
	}
	
	AttachToComponent(WeaponOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, name);
}

void AWeaponBasic::TakeAttack()
{
	
	if (Role<ROLE_Authority)
	{
		Server_Fire();
	}
}

void AWeaponBasic::BeginAttack()
{
	FirstDelay = FMath::Max(LastAttackTime - GetWorld()->TimeSeconds + TimeBetweenAttacks,0.0f);
	GetWorld()->GetTimerManager().SetTimer(attackTimeBetweenHandle,this,&AWeaponBasic::TakeAttack,TimeBetweenAttacks,true,FirstDelay);
}

void AWeaponBasic::StopAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(attackTimeBetweenHandle);
}

void AWeaponBasic::SetDamage(AActor* actor)
{
	if (!WeaponOwner)
	{
		return;
	}
	FPointDamageEvent damageEvent;
	actor->TakeDamage(3, damageEvent, WeaponOwner->GetController(), this);
	ACharacter* character = Cast<ACharacter>(actor);
	if (DamageSound&&character)
	{
		UGameplayStatics::PlaySound2D(this, DamageSound);
		//UE_LOG(LogTemp, Log, TEXT("    %s"), *actor->GetName());
	}
	
}


void AWeaponBasic::OnRef_HitScanTrace()
{

}

//server

void AWeaponBasic::Server_Fire_Implementation()
{
	TakeAttack();
}

bool AWeaponBasic::Server_Fire_Validate()
{
	return true;
}

//这种事个别不用在头文件表明的函数，Unreal头文件工具已经帮忙包含过了这些函数
void AWeaponBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AWeaponBasic, FHitScanTrace);

	//用带条件的这条，可以加上 条件_忽略自身
	DOREPLIFETIME_CONDITION(AWeaponBasic, HitScanTrace,COND_SkipOwner);
}
