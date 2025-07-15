// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/SnakeBase.h"
#include "Game/Character/PlayerPawn.h"
#include "Game/Character/SnakeElementBase.h"
#include "Level/SpawnerBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(SnakeImmortality)
DEFINE_LOG_CATEGORY(CollisionMode)

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HungerSnakeTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("HungerSnakeTimelineComp"));

	//Defaults Snake's value
	ElementSize = 100.f;
	MovementSpeed = 0.3f;
	LastMoveDirection = EMovementDirection::DOWN;
	bIsMove = true;
	bIsImmortal = true;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Binding float track to our UpdateTimelineComp Function's output
	UpdateFunctionFloat.BindDynamic(this, &ASnakeBase::UpdateTimelineComp);
	//If we have a float curve, bind it's graph to our update function
	if (HungerSnakeTimelineCurve)
	{
		HungerSnakeTimelineComponent->AddInterpFloat(HungerSnakeTimelineCurve, UpdateFunctionFloat);
	}
	//Binding Timeline's Finished function
	if (HungerSnakeTimelineComponent)
	{
		FOnTimelineEventStatic onTimelineFinishedCallback;
		onTimelineFinishedCallback.BindUFunction(this, FName("TimelineFinishedFunc"));
		HungerSnakeTimelineComponent->SetTimelineFinishedFunc(onTimelineFinishedCallback);
	}

	HungerSnakeTimelineComponent->Play();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(ElementsNum);
	SetImmortal(); //NoCollision
	TimerResetImmortal(); //CollisionEnabled
}

void ASnakeBase::UpdateTimelineComp(float Output)
{
	HungerSnakeScl = Output;
	SetHungerSnakePercent(HungerSnakeScl);
	FString HungerSnakeStr = FString::SanitizeFloat(HungerSnakeScl);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, HungerSnakeStr);
	}
}

void ASnakeBase::TimelineFinishedFunc()
{
	AddWidgetRestartLevel();
	Destroy();
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int _ElementsNum)
{
	for (int i = 0; i < _ElementsNum; ++i)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElemType();
		}
	}
}

void ASnakeBase::AddNewElement(int _ElementsNum)
{
	for (int i = 0; i < _ElementsNum; ++i)
	{
		int LastElemIndex = SnakeElements.Num() - 1;
		ASnakeElementBase* LastElem = SnakeElements[LastElemIndex];
		FVector LastElemLocation = LastElem->GetActorLocation();
		FTransform NewTransform(LastElemLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		SnakeElements.Add(NewSnakeElem);
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);

	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y -= ElementSize;
		break;
	}

	if (!bIsImmortal)
	{
		SnakeElements[0]->ToggleCollision();
		UE_LOG(CollisionMode, Log, TEXT("No Collision"));
	}

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);

	if (!bIsImmortal)
	{
		SnakeElements[0]->ToggleCollision();
		UE_LOG(CollisionMode, Log, TEXT("Collision enabled"));
	}

	bIsMove = false;
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
}

void ASnakeBase::SetImmortal()
{
	bIsImmortal = true;
	SnakeElements[0]->ToggleCollision();
	UE_LOG(SnakeImmortality, Log, TEXT("Snake is immortal"));
}

void ASnakeBase::ResetImmortal()
{
	bIsImmortal = false;
	SnakeElements[0]->ToggleCollision();
	UE_LOG(SnakeImmortality, Log, TEXT("Snake is not immortal"));
}

void ASnakeBase::TimerResetImmortal()
{
	GetWorldTimerManager().SetTimer(ImmortalTimerHandle, this, &ASnakeBase::ResetImmortal, 2.0f, false);
}

void ASnakeBase::SetBarrierCollision()
{
	SnakeElements[0]->BarrierCollisionDisabled();
}

void ASnakeBase::ResetBarrierCollision()
{
	SnakeElements[0]->BarrierCollisionEnabled();
}

void ASnakeBase::TimerResetBarrierCollisionFunc()
{
	GetWorldTimerManager().SetTimer(ImmortalTimerHandle, this, &ASnakeBase::ResetBarrierCollision, IgnoreBarrierTime, false);
}

void ASnakeBase::SetMovementSpeed(float Value)
{
	SetActorTickInterval(MovementSpeed * Value);
}

void ASnakeBase::ResetMovementSpeed()
{
	SetActorTickInterval(MovementSpeed);
}

void ASnakeBase::TimerResetMovementSpeed()
{
	GetWorld()->GetTimerManager().SetTimer(ResetSpeedTimerHandle, this, &ASnakeBase::ResetMovementSpeed, 5.0f, false);
}

//Timeline functions
void ASnakeBase::PlayfromStartTimline()
{
	HungerSnakeTimelineComponent->PlayFromStart();
}

void ASnakeBase::SetHungerSnakePercent(float _HungerSnake)
{
	HungerSnakePercent = _HungerSnake / HungerSnakeMax;
}

void ASnakeBase::AddWidgetRestartLevel_Implementation()
{
	UGameplayStatics::SetGamePaused(this, true);
	Player = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(this, PlayerClass));
	Player->bEndGame = true;
}

void ASnakeBase::AddWidgetWinGame_Implementation()
{
	auto Spawner = Cast<ASpawnerBase>(UGameplayStatics::GetActorOfClass(this, SpawnerClass));
	auto Grid = Spawner->FloorGrid.Num();
	if (SnakeElements.Num() == Grid)
	{
		UGameplayStatics::SetGamePaused(this, true);
		Player = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(this, PlayerClass));
		bIsWin = Player->bWinGame = true;
	}
}

