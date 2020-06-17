// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBasic.generated.h"


USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> Surface;

	UPROPERTY()
	FVector_NetQuantizeNormal TraceTo;

};


UCLASS()
class UEPROJECT05_API AWeaponBasic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBasic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
		class ACharacter* WeaponOwner;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* WeaponComp;
	UPROPERTY(EditAnywhere)
		class USoundBase* DamageSound;

	//Attack per minute for weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RotaOfAttack;

	float TimeBetweenAttacks;
	float LastAttackTime;
	float FirstDelay;

	bool bSword;

	FTimerHandle attackTimeBetweenHandle;

public:
	class ACharacter* GetPlayer() { return WeaponOwner; }
	bool IsHasOwner() { return WeaponOwner!=nullptr; }
	void SetWeaponOwner(class ACharacter* player);
	void SetWeaponAttach(FName name);

	bool IsSword() { return bSword; }

	virtual void TakeAttack();
	virtual void BeginAttack();
	virtual void StopAttack();

	
	class USkeletalMeshComponent* GetWeaponComp() const { return WeaponComp; }

	void SetDamage(AActor* actor);


protected:
	virtual void WeaponEffect() {};


public:

	//Server
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();


	UPROPERTY(ReplicatedUsing= OnRef_HitScanTrace)
		FHitScanTrace HitScanTrace;
	UFUNCTION()
		void OnRef_HitScanTrace();
};
