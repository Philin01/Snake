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
			if (Snake->bDoubleFoodEnable)
			{
				Snake->AddNewElement(2);
				Snake->ScoreCounter += 2;
			}
			else
			{
				Snake->AddNewElement();
				Snake->ScoreCounter += 1;
			}
			Snake->SetMovementSpeed(2.0f);
			Snake->bSpeedDownEnable = true;
			Snake->TimerResetMovementSpeed();
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}
