// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Monster_Character.generated.h"

class UACTHealthComponent;


UCLASS()
class UEPROJECT05_API AMonster_Character : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster_Character();

	AMonster_Character(const class FObjectInitializer& ObjectInitializer);

	class UBehaviorTree* GetBehaviorTree()const { return BTree; }

	
//Test
protected:
	UPROPERTY(VisibleAnywhere)
	UACTHealthComponent* HealthComp;


protected:
	UFUNCTION()
	void OnHealthChange(UACTHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void Onseeplayer(APawn* pawn);
	UFUNCTION(BlueprintCallable)
		void OnHearNoise(APawn* PawnInstigetor, const FVector& Location, float Valume);//参数1.噪音发出者 2.噪音发出位置  3.噪音强度
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	
	UFUNCTION()
		void OnUpdatePerceptionActors(const TArray<AActor*>& UpdataActor);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsSpring() { return bSpring; }
	bool IsHoldGun() { return bHoldGun; }
	class AWeapon_Gun* GetHoldGun() { return HoldGun; }
	class AActor* GetAlarm() { return Alarm; };
	void NotSpring();
	bool isDead() { return bMyDead; }
	class AWeapon_Sword* GetHoldSword()const { return HoldSword; }
protected:

	UPROPERTY(visibleAnywhere)
	class UPawnSensingComponent* Sensingcomp;//感知功能组件
	UPROPERTY(BlueprintReadWrite)
		int32 HP;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="Monster")
		int32 MaxHP;
	//Weapon spread random
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster",meta=(ClampMin=0))
		float GunSpread;




	UPROPERTY(EditAnywhere)
		class UBehaviorTree* BTree;
	UPROPERTY()
		class UAIPerceptionComponent* AIPerceptionComp;
	UPROPERTY()
		bool bSpring;
	UPROPERTY()
		bool bMyDead;
	UPROPERTY()
		class AWeapon_Gun* HoldGun;
	UPROPERTY()
		class AWeapon_Sword* HoldSword;
	UPROPERTY(EditAnywhere)
		uint8 bHoldGun : 1;
	UPROPERTY()
		class AActor* Alarm;
	UPROPERTY(EditAnywhere)
		uint8 bIdle : 1;
	
	class UMaterialInstanceDynamic* MatInst;
};
