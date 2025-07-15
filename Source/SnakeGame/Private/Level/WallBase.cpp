// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/WallBase.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"
#include "Game/Character/SnakeBase.h"

// Sets default values
AWallBase::AWallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("MeshMaterial"));
}

// Called when the game starts or when spawned
void AWallBase::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->SetMaterial(0, MeshMaterial);
}

// Called every frame
void AWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallBase::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddWidgetRestartLevel();
			Snake->Destroy();
		}
	}
}

