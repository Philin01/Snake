// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Interact/FoodBase.h"
#include "FoodSpeedDownDebuf.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEGAME_API AFoodSpeedDownDebuf : public AFoodBase
{
	GENERATED_BODY()

public:
	
	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
