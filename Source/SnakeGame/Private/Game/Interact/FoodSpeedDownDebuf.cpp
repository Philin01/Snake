// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interact/FoodSpeedDownDebuf.h"
#include "Game/Character/SnakeBase.h"

void AFoodSpeedDownDebuf::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddNewElement();
			Snake->SetMovementSpeed(2.0f);
			Snake->TimerResetMovementSpeed();
			Snake->ScoreCounter += 1;
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}
