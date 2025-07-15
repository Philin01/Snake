// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class ASnakeBase;

UCLASS()
class SNAKEGAME_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	//Pawn's variables
	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* PawnCamera;
	//Snake's variables
	UPROPERTY(BlueprintReadWrite)
	ASnakeBase* SnakeActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBase> SnakeActorClass;
	//Gamemode variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bStartGame;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bEndGame;
	UPROPERTY()
	bool bWinGame;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CreateSnakeActor();

	UFUNCTION()
	void HandlerPlayerVerticalInput(float value);
	UFUNCTION()
	void HandlerPlayerHorizontalInput(float value);
	UFUNCTION(BlueprintNativeEvent)
	void PlayerStartGameInputFunc();
	void PlayerStartGameInputFunc_Implementation();

	void SetPaused(bool bIsPaused);
	void RestartSnakeGame();
};
