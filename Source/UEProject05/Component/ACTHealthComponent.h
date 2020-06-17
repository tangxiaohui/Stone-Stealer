// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACTHealthComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignatrue, UACTHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT05_API UACTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACTHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly,Category="Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float DefaultHealth;

	
	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

public:
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnHealthChangeSignatrue OnHealthChanged;
};
