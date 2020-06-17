// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_Character.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/PlayerController.h"
#include "UI/ACTHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Player/ACTCharacter.h"
#include "UEProject05GameModeBase.h"

// Sets default values
ANPC_Character::ANPC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Sensingcomp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("sening"));
}

// Called when the game starts or when spawned
void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC_Character::Onseeplayer(APawn* pawn)
{
	AUEProject05GameModeBase* GM = Cast<AUEProject05GameModeBase>(GetWorld()->GetAuthGameMode());
	
	AAIController* NPC = Cast<AAIController>(GetController());
	if (NPC)
	{
		
		NPC->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), pawn);
		bfind = true;
		Player = Cast<AACTCharacter>(pawn);
		Player->SetTaskNPC(this);
		GM->SetPlayer(Player);
		if (!Player->GetIsTask())
		{
			Player->HaveFindTask();
		}
	}
	APlayerController* pc = Cast<APlayerController>(pawn->GetController());
	
	if (!pc)
	{
		return;
	}
	AACTHUD* hud = Cast<AACTHUD>(pc->GetHUD());
	if (!hud||Player->GetIsTask())
	{
		return;
	}
	hud->GetTaskUI();
}

void ANPC_Character::OnHearNoise(APawn* PawnInstigetor, const FVector& Location, float Valume)
{
	AAIController* NPC= Cast<AAIController>(GetController());
	if (NPC)
	{
		NPC->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PawnInstigetor);
		bfind = true;
		Player = Cast<AACTCharacter>(PawnInstigetor);
		if (!Player->GetIsTask())
		{
			Player->HaveFindTask();
		}
	}
	
}

// Called every frame
void ANPC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Hind)
	{
		return;
	}
	USkeletalMeshComponent* mesh;
	mesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	FVector StartPos = mesh->GetSocketLocation(TEXT("Hind"));
	Hind = GetWorld()->SpawnActor<AActor>(LoadClass<AActor>(this, TEXT("Blueprint'/Game/ACTGame/Blueprints/NPC_BP/BP_Hind.BP_Hind_C'")), StartPos, FRotator::ZeroRotator);

	Hind->AttachToComponent(mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hind"));
	UE_LOG(LogTemp, Log, TEXT("mmmmmmmm"));
}


// Called to bind functionality to input
void ANPC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC_Character::DeleteHind()
{
	if (Hind)
	{
		Hind->Destroy();
	}
}

