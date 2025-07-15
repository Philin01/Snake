// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/SnakeElementBase.h"
#include "Game/Character/SnakeBase.h"
#include "Runtime/Engine/Classes/Components/MeshComponent.h"

// Sets default values
ASnakeElementBase::ASnakeElementBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SnakeHeadMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("SnakeHeadMaterial"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ASnakeElementBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeElementBase::SetFirstElemType_Implementation()
{
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeElementBase::HandleBeginOverlap);
	MeshComponent->SetMaterial(0, SnakeHeadMaterial);
}

void ASnakeElementBase::HandleBeginOverlap
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	if (IsValid(SnakeOwner))
	{
		SnakeOwner->SnakeElementOverlap(this, OtherActor);
	}
}

void ASnakeElementBase::ToggleCollision()
{
	if (MeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASnakeElementBase::BarrierCollisionDisabled()
{
	if (MeshComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap)
	{
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	}
}

void ASnakeElementBase::BarrierCollisionEnabled()
{
	if (MeshComponent->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Ignore)
	{
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	}
}

void ASnakeElementBase::Interact(AActor* Interactor, bool bIsHead)
{
	ASnakeBase* Snake = Cast<ASnakeBase>(Interactor);
	if (IsValid(Snake))
	{
		Snake->AddWidgetRestartLevel();
		Snake->Destroy();
	}
}

