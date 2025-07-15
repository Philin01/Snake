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
			Snake->AddNewElement();
			Snake->SetBarrierCollision();
			Snake->TimerResetBarrierCollisionFunc();
			Snake->ScoreCounter += 1;
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}