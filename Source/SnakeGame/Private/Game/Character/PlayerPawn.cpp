// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/PlayerPawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Game/Character/SnakeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	PawnCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	RootComponent = PawnCamera;
	bStartGame = false;
	bEndGame = false;
	bWinGame = false;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetPaused(true);
	SetActorRotation(FRotator(-50, 0, 0));
	SetActorLocation(FVector(-750, 0, 900));
	CreateSnakeActor();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawn::HandlerPlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawn::HandlerPlayerHorizontalInput);
	PlayerInputComponent->BindAction
	(
		"StartGame",
		EInputEvent::IE_Pressed,
		this,
		&APlayerPawn::PlayerStartGameInputFunc
	).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction
	(
		"RestartGame",
		EInputEvent::IE_Pressed,
		this,
		&APlayerPawn::RestartSnakeGame
	).bExecuteWhenPaused = true;
}

void APlayerPawn::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawn::HandlerPlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor) && !SnakeActor->bIsMove)
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::DOWN)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::UP;
			SnakeActor->bIsMove = true;
		}
		if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::UP)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::DOWN;
			SnakeActor->bIsMove = true;
		}
	}
}

void APlayerPawn::HandlerPlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor) && !SnakeActor->bIsMove)
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::LEFT)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::RIGHT;
			SnakeActor->bIsMove = true;
		}
		if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::RIGHT)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::LEFT;
			SnakeActor->bIsMove = true;
		}
	}
}

void APlayerPawn::PlayerStartGameInputFunc_Implementation()
{
	if (bStartGame == false)
	{
		SetPaused(false);
		bStartGame = true;
	}
}

void APlayerPawn::SetPaused(bool bIsPaused)
{
	UGameplayStatics::SetGamePaused(this, bIsPaused);
}

void APlayerPawn::RestartSnakeGame()
{
	if (bEndGame == true || bWinGame == true)
	{
		FString Restart = "RestartLevel";

		UKismetSystemLibrary::ExecuteConsoleCommand
		(
			this,
			Restart
		);
	}
}

