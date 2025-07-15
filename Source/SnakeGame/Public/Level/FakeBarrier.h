// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/BarrierBase.h"
#include "FakeBarrier.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API AFakeBarrier : public ABarrierBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
};
