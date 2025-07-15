// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/FakeBarrier.h"

void AFakeBarrier::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
