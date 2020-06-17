// Fill out your copyright notice in the Description page of Project Settings.

#include "ACTHealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UACTHealthComponent::UACTHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	DefaultHealth = 100;

	SetIsReplicated(true);
}


// Called when the game starts
void UACTHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Only authority
	//if (GetOwnerRole()==ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UACTHealthComponent::TakeAnyDamage);

		}
	}




	Health = DefaultHealth;
	
}


void UACTHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(this,Health,Damage,DamageType, InstigatedBy, DamageCauser);
		//UE_LOG(LogTemp, Log, TEXT("  OnHealthChanged"));
	}
}
  
// Called every frame
void UACTHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

