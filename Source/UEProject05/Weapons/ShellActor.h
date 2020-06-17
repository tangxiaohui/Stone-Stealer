// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShellActor.generated.h"

class ACharacter;
class AWeapon_Gun;

UCLASS()
class UEPROJECT05_API AShellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShellActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	class UStaticMeshComponent* GetMesh() { return ShellComp; }

	void SetShellOwner(ACharacter* shellOwner) { this->ShellOwner = shellOwner; }
	void SetOwnerWeapon(AWeapon_Gun* Weapon) {this->OwnerWeapon = Weapon; }
protected:
	//UPROPERTY(VisibleAnywhere)
	//	class USceneComponent* Scenes;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ShellComp;
	//UPROPERTY(VisibleAnywhere)
	//class UProjectileMovementComponent* ProjectileCom;


	UPROPERTY(BlueprintReadOnly,Category="Bullet")
		ACharacter* ShellOwner;
	UPROPERTY(BlueprintReadOnly, Category = "Bullet")
		AWeapon_Gun* OwnerWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
		class UParticleSystem* AttackImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
		class UParticleSystem* DefaultImpactEffect;


protected:

	UFUNCTION()
		void OnComponentHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//UFUNCTION()
	//	void OnComponentBeginOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//UFUNCTION()
	//	void OnComponentEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
