// Fill out your copyright notice in the Description page of Project Settings.

#include "ShellActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/Character.h>
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UEProject05.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon_Gun.h"

// Sets default values
AShellActor::AShellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//Scenes = CreateDefaultSubobject<USceneComponent>(TEXT("SceneMesh"));
	//SetRootComponent(Scenes);
    ShellComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShellMesh"));
	//ProjectileCom=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	
	//ShellComp->SetupAttachment(Scenes);
	SetRootComponent(ShellComp);

	//ShellComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//ShellComp->SetCollisionResponseToAllChannels(ECR_Block);
	//ShellComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	ShellComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetReplicates(true);
	
}

// Called when the game starts or when spawned
void AShellActor::BeginPlay()
{
	Super::BeginPlay();
	
	//ShellComp->OnComponentBeginOverlap.AddDynamic(this,&AShellActor::OnComponentBeginOverlapEvent);
	ShellComp->OnComponentHit.AddDynamic(this, &AShellActor::OnComponentHitEvent);
	//ShellComp->OnComponentBeginOverlap.AddDynamic(this, &AShellActor::OnComponentBeginOverlapEvent);

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffect, GetActorLocation(), FRotator::ZeroRotator);

	//ProjectileCom->InitialSpeed = 2000;

	SetLifeSpan(3.0f);


	//UE_LOG(LogTemp, Log, TEXT(" Weapon Owner : %d"),OwnerWeapon!=nullptr);
}

// Called every frame
//void AShellActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AShellActor::OnComponentHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.GetActor()==ShellOwner||Hit.GetActor() == OwnerWeapon)
	{
		return;
	}
	
	if (ShellOwner)
	{
		FPointDamageEvent pointEvent;
		OtherActor->TakeDamage(FMath::FRandRange(3, 5), pointEvent, ShellOwner->GetController(), this);
	}
	//UE_LOG(LogTemp, Log, TEXT(" Bullet has Boom"));

	//Effect
	UParticleSystem* ImpactEffect=nullptr;
	EPhysicalSurface PhysSurfaca=UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	FVector effectSpawnPos= Hit.ImpactPoint;
	switch (PhysSurfaca)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		ImpactEffect = AttackImpactEffect;
		effectSpawnPos += Hit.ImpactNormal * 50;
		break;
	default:
		ImpactEffect = DefaultImpactEffect;
		break;
	}

	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,effectSpawnPos, Hit.ImpactNormal.Rotation());
	}

	Destroy();
}

//void AShellActor::OnComponentBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//		
//
//	if (ShellOwner)
//	{
//		FPointDamageEvent pointEvent;
//		OtherActor->TakeDamage(FMath::FRandRange(3, 5), pointEvent, ShellOwner->GetController(), this);
//	}
//	//UE_LOG(LogTemp, Log, TEXT(" Bullet has Boom"));
//	UE_LOG(LogTemp, Log, TEXT(" Shell  effect"));
//	//Effect
//	UParticleSystem* ImpactEffect = nullptr;
//	EPhysicalSurface PhysSurfaca = UPhysicalMaterial::DetermineSurfaceType(SweepResult.PhysMaterial.Get());
//	switch (PhysSurfaca)
//	{
//	case SURFACE_FLESHDEFAULT:
//	case SURFACE_FLESHVULNERABLE:
//		ImpactEffect = AttackImpactEffect;
//		break;
//	default:
//		ImpactEffect = DefaultImpactEffect;
//		break;
//	}
//
//	if (ImpactEffect)
//	{
//		//UE_LOG(LogTemp, Log, TEXT(" Shell  effect spawn success"));
//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, SweepResult.ImpactPoint + SweepResult.ImpactNormal * 50, SweepResult.ImpactNormal.Rotation());
//	}
//	else
//	{
//		UE_LOG(LogTemp, Log, TEXT(" Shell  effect spawn fail"));
//	}
//
//	Destroy();
//}


