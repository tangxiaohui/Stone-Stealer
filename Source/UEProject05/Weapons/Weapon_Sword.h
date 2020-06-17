// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBasic.h"
#include "Weapon_Sword.generated.h"

/**
 * 
 */
class UAnimMontage;


UCLASS()
class UEPROJECT05_API AWeapon_Sword : public AWeaponBasic
{
	GENERATED_BODY()
public:
	AWeapon_Sword();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//UFUNCTION()
	//	void OnComponentHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnComponentBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//UFUNCTION()
	//	void OnComponentEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage01;
	//UPROPERTY()
	TArray<UAnimMontage*> AttackMontageArray;

	FTimerHandle resetComboHandle;
		
	uint32 AttackNum;
	uint32 AttackMaxNum;
	float MontagePlayTime;
	float ComboTime;
	bool bCanPlayMontage;
	bool bContinueCombo;

	float BeginComboTrace;
	float EndComboTrace;
	//UPROPERTY(EditAnywhere,Category="AttackMontage")
	bool bComboTrace;
	//UPROPERTY(EditAnywhere, Category = "AttackMontage")
	bool bBeginComboTrace;

	bool bCombo;
public:
	virtual void TakeAttack() override;
	virtual void BeginAttack() override;
	virtual void StopAttack() override;

protected:
	void PlayAttactMontage();
	void PlayAttackMontage1();
	//void CanPlayMontageCallBack();
	void ResetComboCallBack();
};
