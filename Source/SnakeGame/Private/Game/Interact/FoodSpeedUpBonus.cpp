// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interact/FoodSpeedUpBonus.h"
#include "Game/Character/SnakeBase.h"

void AFoodSpeedUpBonus::Interact(AActor* Interactor, bool bIsHead)
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
			Snake->SetMovementSpeed(0.5f);
			Snake->bSpeedUpEnable = true;
			Snake->TimerResetMovementSpeed();
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}
