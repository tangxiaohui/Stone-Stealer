// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBasic.h"
#include "Camera/CameraShake.h"
#include "Weapon_Gun.generated.h"

/**
 * 
 */
class UParticleSystem;

	


UCLASS()
class UEPROJECT05_API AWeapon_Gun : public AWeaponBasic
{
	GENERATED_BODY()
public:
	AWeapon_Gun();

public:
	virtual void TakeAttack() override;

	virtual void WeaponEffect() override;

	void SetGunSpread(float gunSpread) { BulletSpread = gunSpread; }

protected:
	UPROPERTY(BlueprintReadOnly,Category="Weapon")
		class AShellActor* Shells;

	UPROPERTY()
		class USoundBase* FireSound;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* AttackEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<AShellActor> ShellClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> CameraShakeTest;


	FVector MuzzlePos;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector TracerEnd;
	
protected:
	void MonsterAttack();
	void PlayerAttack();

private:
	UFUNCTION(BlueprintCallable,Category="Weapon")
	void TestFunction();


};
