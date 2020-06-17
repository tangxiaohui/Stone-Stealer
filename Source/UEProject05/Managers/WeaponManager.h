// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Engine/DataTable.h>
#include "WeaponManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Shouliu,
	EWT_Shouqiang,
	EWT_Buqiang,
	EWT_Food,
	EWT_Sword,
};


USTRUCT(BlueprintType)
struct FWeaponBasicInfo:public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* RelaodAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AmoonNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireInterVal;
};

USTRUCT(BlueprintType)
struct FWeaponInfo_Gun :public FWeaponBasicInfo
{
	GENERATED_USTRUCT_BODY();
	
};

USTRUCT(BlueprintType)
struct FWeaponInfo_Sword :public FWeaponBasicInfo
{
	GENERATED_USTRUCT_BODY();

};


UCLASS()
class UEPROJECT05_API UWeaponManager : public UObject
{
	GENERATED_BODY()
	
};
