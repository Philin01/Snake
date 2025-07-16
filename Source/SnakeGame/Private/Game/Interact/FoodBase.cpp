// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Interact/FoodBase.h"
#include "Components/StaticMeshComponent.h"
#include "Game/Character/SnakeBase.h"

// Sets default values
AFoodBase::AFoodBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = MeshComponent;
	SetRotationTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Rotation"));
	FoodMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));
}

// Called when the game starts or when spawned
void AFoodBase::BeginPlay()
{
	Super::BeginPlay();

	//Binding float track to our Timeline' UpdateFunc
	UpdateFuncFloat.BindDynamic(this, &AFoodBase::UpdateFunc);
	//If wa have float track, bind it' graph to our UpdateFunc
	if (RotationCurve)
	{
		SetRotationTimelineComponent->AddInterpFloat(RotationCurve, UpdateFuncFloat);
	}
	//Binding FinishFunc to our Timeline
	if (SetRotationTimelineComponent)
	{
		TimelineFinishedEvent.BindUFunction(this, FName("FinishedFunc"));
		SetRotationTimelineComponent->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
	MeshComponent->SetMaterial(0, FoodMaterial);
	VisibleTimer();
	DestroyFoodTimer();
	SetRotationTimelineComponent->Play();	
}

void AFoodBase::UpdateFunc(float Output)
{
	FRotator NewRotation(0, Output, 0);
	SetActorRotation(NewRotation);
}

void AFoodBase::FinishedFunc()
{
	SetRotationTimelineComponent->PlayFromStart();
}

// Called every frame
void AFoodBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodBase::Interact(AActor* Interactor, bool bIsHead)
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
			//Call our Delegate
			SpawnFood.Broadcast();
			//Reboot Timeline
			Snake->PlayfromStartTimline();
			Snake->AddWidgetWinGame();
			Destroy();
		}
	}
}

void AFoodBase::VisibleTimer()
{
	int Delay = LifeTime - 5;
	GetWorld()->GetTimerManager().SetTimer(VisibleTimerHandle, this, &AFoodBase::ToggleVis, 0.3f, true, Delay);
}

void AFoodBase::ToggleVis()
{
	MeshComponent->ToggleVisibility();
}

void AFoodBase::DestroyFood()
{
	SpawnFood.Broadcast();
	Destroy();
}

void AFoodBase::DestroyFoodTimer()
{
	GetWorld()->GetTimerManager().SetTimer
	(
		DestroyTimerHandle,
		this,
		&AFoodBase::DestroyFood,
		LifeTime,
		false
	);
}

