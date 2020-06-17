// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster_AnimInstance.h"
#include "GameFramework/Character.h"
#include "Monster_Character.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

void UMonster_AnimInstance::NativeBeginPlay()
{
	NormalPosIndex = FMath::RandRange(0, 1);
	bDead = false;
}

void UMonster_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ACharacter* cha = Cast<ACharacter>(TryGetPawnOwner());
	if (!cha)
	{
		return;
	}
	Speed = cha->GetVelocity().Size2D();
	AMonster_Character* Mon = Cast<AMonster_Character>(TryGetPawnOwner());

	if (!Mon)
	{
		return;
	}
	bDead = Mon->isDead();
	bHoldGun = Mon->IsHoldGun();
		if (!Mon->IsSpring() && Speed <= 0.2)
		{
			if ((TickValue += DeltaSeconds) >= 1)
			{
				bNormalIdle = true;

			}
			else
			{
				bNormalIdle = false;
			}
		}
		else
		{
			TickValue = 0;
			bNormalIdle = false;
		}
		FVector PlayerPos;
		
		AController* MC = Cast<AController>(Mon->GetController());
		AAIController* Cont = Cast<AAIController>(MC);
		if (Mon->IsSpring()&&Cont)
		{
			UObject* obj =Cont->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
			AActor* act = Cast<AActor>(obj);
			if (act)
			{
				PlayerPos = act->GetActorLocation();
			}
			FVector dir = (act->GetActorLocation()+FVector(0,0,130))- Mon->GetActorLocation();
			FVector LocDir = Mon->ActorToWorld().InverseTransformVectorNoScale(dir);
			FRotator Rot = LocDir.Rotation();
			Pich = Rot.Pitch;
			//Rot = CalculateDirection(Mon->GetVelocity(), Mon->GetActorRotation());
		}

}
	

	


void UMonster_AnimInstance::AnimNotify_MoveStep()
{
	TickValue = 0;
	bNormalIdle = false;
	NormalPosIndex = FMath::RandRange(0, 1);
}

