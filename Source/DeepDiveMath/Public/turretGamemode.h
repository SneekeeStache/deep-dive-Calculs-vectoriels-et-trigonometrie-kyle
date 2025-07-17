// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "turretGamemode.generated.h"

/**
 * 
 */
UCLASS()
class DEEPDIVEMATH_API AturretGamemode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="turret")
	TArray<AActor*> Targets;
};
