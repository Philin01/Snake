// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/BarrierBase.h"
#include "Game/Character/SnakeBase.h"
#include "Level/SpawnerBase.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"

// Sets default values
ABarrierBase::ABarrierBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	MeshMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("MeshMaterial"));
}

// Called when the game starts or when spawned
void ABarrierBase::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->SetMaterial(0, MeshMaterial);
	VisibleBarrierTimer();
	DestroyTimer();
}

// Called every frame
void ABarrierBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarrierBase::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead) 
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake)) {
			Snake->AddWidgetRestartLevel();
			Snake->Destroy();
		}
	}
}

void ABarrierBase::VisibleBarrierTimer()
{
	float Delay = LifeTime - 3;
	GetWorld()->GetTimerManager().SetTimer(VisibleBarrierTimerHandle, this, &ABarrierBase::ToggleVisBarrier, 0.3f, true, Delay);
}

void ABarrierBase::ToggleVisBarrier()
{
	MeshComponent->ToggleVisibility();
}

void ABarrierBase::DestroyTimer()
{
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABarrierBase::DestroyObj, LifeTime, false);
}

void ABarrierBase::DestroyObj()
{
	Destroy();
}

