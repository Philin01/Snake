// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interact/DoubleFoodBonus.h"
#include "Game/Character/SnakeBase.h"

void ADoubleFoodBonus::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddNewElement(2);
			Snake->ScoreCounter += 2;
			Snake->PlayfromStartTimline();
			SpawnFood.Broadcast();
			Destroy();
		}
	}
}
