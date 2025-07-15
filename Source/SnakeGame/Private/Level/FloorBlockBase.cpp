// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/FloorBlockBase.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"

// Sets default values
AFloorBlockBase::AFloorBlockBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	FirstBlockMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("FirstBlockMaterial"));
	SecondBlockMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("SecondBlockMaterial"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AFloorBlockBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorBlockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorBlockBase::SetBlockMaterial(int _BlockIndex)
{
	if (_BlockIndex % 2) 
	{
		MeshComponent->SetMaterial(0, FirstBlockMaterial);
	}
	else 
	{
		MeshComponent->SetMaterial(0, SecondBlockMaterial);
	}
}

