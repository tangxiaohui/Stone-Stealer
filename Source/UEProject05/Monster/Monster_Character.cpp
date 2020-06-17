// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster_Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Mons_CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Weapons/Weapon_Gun.h"
#include "Weapons/Weapon_Sword.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "UEProject05.h"
#include "Component/ACTHealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonster_Character::AMonster_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


}

AMonster_Character::AMonster_Character(const class FObjectInitializer& Initializer)

	:Super(Initializer.SetDefaultSubobjectClass<UMons_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	Sensingcomp=CreateDefaultSubobject<UPawnSensingComponent>(TEXT("sening"));

	HealthComp=CreateDefaultSubobject<UACTHealthComponent>(TEXT("HealthComp"));
	bSpring = false;  
	MaxHP = 20;
	HP = MaxHP;
	bMyDead = false;
	GunSpread = 4.0f;
}


// Called when the game starts or when spawned
void AMonster_Character::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp->OnHealthChanged.AddDynamic(this, &AMonster_Character::OnHealthChange);


	AAIController* Cont = Cast<AAIController>(GetController());
	if (bIdle)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("Isidle"), 1);
	}
	
	
	if (bHoldGun)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHoldGun"), 1);
		HoldGun = Cast<AWeapon_Gun>(GetWorld()->SpawnActor(LoadClass<AWeapon_Gun>(this, TEXT("Blueprint'/Game/ACTGame/Blueprints/Weapon/BP_GunTest.BP_GunTest_C'"))));
		if (!HoldGun) return;
		HoldGun->SetWeaponOwner(this);
		HoldGun->SetWeaponAttach(TEXT("HoldWeapon"));//把枪放置在手上的凹槽
		HoldGun->SetGunSpread(GunSpread);
	}
	else
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHoldGun"), 0);
		HoldSword = Cast<AWeapon_Sword>(GetWorld()->SpawnActor(LoadClass<AWeapon_Sword>(this, TEXT("Blueprint'/Game/ACTGame/Blueprints/Weapon/BP_Sword.BP_Sword_C'"))));
		if (!HoldGun) return;
		HoldSword->SetWeaponOwner(this);
		HoldSword->SetWeaponAttach(TEXT("HoldSword"));//把枪放置在手上的凹
	}
	
	
	AIPerceptionComp = Cast<UAIPerceptionComponent>(FindComponentByClass(UAIPerceptionComponent::StaticClass()));
	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AMonster_Character::OnUpdatePerceptionActors);
	}
	


}


void AMonster_Character::OnHealthChange(UACTHealthComponent* HealthComponent, float Health,
	float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	//UE_LOG(LogTemp,Log,TEXT("Run OnHealthChange"));


	if (Health<=0)
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (MatInst==nullptr)
	{
		MatInst = GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, GetMesh()->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken",GetWorld()->TimeSeconds);
	}
	
	
}

void AMonster_Character::Onseeplayer(APawn* pawn)
{
	AAIController* Mon = Cast<AAIController>(GetController());
	if (Mon)
	{
		Mon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), pawn);
	}
	
	bSpring = true;
	
}

void AMonster_Character::OnHearNoise(APawn* PawnInstigetor, const FVector& Location, float Valume)
{
	AAIController* Mon = Cast<AAIController>(GetController());
	if (Mon)
	{
		Mon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PawnInstigetor);
	}
	//UE_LOG(LogTemp, Log, TEXT("xxxxxxx"));
	bSpring = true;
}

float AMonster_Character::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	

	if (HP<=0)
	{
		return 0;
	}
	AAIController* Cont = Cast<AAIController>(GetController());

	HP -= Damage;
	if (HP<=0)
	{
		bMyDead = true;

		Cont->UnPossess();
		Cont->GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		if (Alarm)
		{
			Alarm->Destroy();
		}

		if (HoldGun)
		{
			HoldGun->AddActorLocalOffset(GetActorForwardVector() * 80);
			HoldGun->SetWeaponOwner(nullptr);
			HoldGun = nullptr;
		}
		
		
		SetLifeSpan(5);
	}
	//UE_LOG(LogTemp, Log, TEXT("  asdf"));
	if (Cont)
	{
		bSpring = true;
		Cont->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), EventInstigator->GetPawn());
		Cont->GetBlackboardComponent()->SetValueAsEnum(TEXT("MonState"), 1);//设置黑板里的1号枚举项
	}
	return 0.0f;
}

void AMonster_Character::OnUpdatePerceptionActors(const TArray<AActor*>& UpdataActor)
{
	/*for (auto item:UpdataActor)
	{
		FActorPerceptionBlueprintInfo Info;
		if (AIPerceptionComp->GetActorsPerception(item,Info))
		{
			if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
			{
				AAIController* Mon = Cast<AAIController>(GetController());
				Mon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Info.Target);
				bSpring = true;
			}
			else if (Info.LastSensedStimuli[1].WasSuccessfullySensed())
			{

				AAIController* Mon = Cast<AAIController>(GetController());
				Mon->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Info.Target);
				bSpring = true;
			}
			else
			{
				UAIBlueprintHelperLibrary::GetAIController(this)->GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));

			}

		}
	}*/
}

ETeamAttitude::Type AMonster_Character::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent)
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

// Called every frame
void AMonster_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Alarm||!bSpring)
	{
		return;
	}
	USkeletalMeshComponent* mesh;
	mesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));		
	FVector StartPos = mesh->GetSocketLocation(TEXT("Alarm"));
	Alarm = GetWorld()->SpawnActor<AActor>(LoadClass<AActor>(this, TEXT("Blueprint'/Game/ACTGame/Blueprints/Monster_BP/BP_Alarm.BP_Alarm_C'")), StartPos, FRotator::ZeroRotator);

	Alarm->AttachToComponent(mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Alarm"));
	//UE_LOG(LogTemp, Log, TEXT("mmmmmmmm"));
}
	


// Called to bind functionality to input
void AMonster_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster_Character::NotSpring()
{
	bSpring = false;
}

