// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interact/FoodBarrierIgnoreBonus.h"
#include "Game/Character/SnakeBase.h"

void AFoodBarrierIgnoreBonus::Interact(AActor* Interactor, bool bIsHead)
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
			Snake->SetBarrierCollision();
			Snake->TimerResetBarrierCollisionFunc();
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}