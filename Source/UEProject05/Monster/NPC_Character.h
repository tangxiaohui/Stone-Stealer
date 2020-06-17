// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_Character.generated.h"

UCLASS()
class UEPROJECT05_API ANPC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Character();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void Onseeplayer(APawn* pawn);
	UFUNCTION(BlueprintCallable)
		void OnHearNoise(APawn* PawnInstigetor, const FVector& Location, float Valume);//参数1.噪音发出者 2.噪音发出位置  3.噪音强度
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	class UBehaviorTree* GetBehaviorTree()const { return NTree; }
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DeleteHind();

	bool GetFind() { return bfind; }
protected:

	UPROPERTY(visibleAnywhere)
		class UPawnSensingComponent* Sensingcomp;//感知功能组件
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* NTree;
	UPROPERTY(EditAnywhere)
		uint8 bfind : 1;
	UPROPERTY()
		class AActor* Hind;
	UPROPERTY()
		class AACTCharacter* Player;
};
