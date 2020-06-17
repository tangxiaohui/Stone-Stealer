// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Monster_Manager.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo :public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* Tree;


};
/**
 * 
 */
UCLASS()
class UEPROJECT05_API UMonster_Manager : public UObject
{
	GENERATED_BODY()
	
};
