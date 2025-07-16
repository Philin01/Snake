// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SnakeImmortality, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(CollisionMode, Log, All)

class ASnakeElementBase;
class APlayerPawn;
class ASpawnerBase;

UENUM()
enum class EMovementDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

UCLASS()
class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	//Snake variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY(BlueprintReadWrite)
	ASnakeElementBase* SnakeElement;

	UPROPERTY(EditDefaultsOnly)
	float ElementSize;

	UPROPERTY(EditAnywhere)
	float MovementSpeed;

	UPROPERTY(EditAnywhere)
	float HungerSnakeMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HungerSnakePercent;

	UPROPERTY(EditAnywhere)
	float HungerSnakeScl;

	UPROPERTY(BlueprintReadWrite)
	EMovementDirection LastMoveDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASnakeElementBase*> SnakeElements;

	UPROPERTY(EditDefaultsOnly)
	int ElementsNum = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ScoreCounter = 0;

	UPROPERTY(EditDefaultsOnly)
	float IgnoreBarrierTime;

	bool bIsMove;

	bool bIsImmortal;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	bool bIsWin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool SpeedUpEnable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool SpeedDownEnable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool BarrierIgnoreEnable;

	FTimerHandle ImmortalTimerHandle;

	FTimerHandle ResetSpeedTimerHandle;

	//Player variables
	APlayerPawn* Player;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerPawn> PlayerClass;

	//Spawner variable
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpawnerBase> SpawnerClass;

	//Timeline variables
	UPROPERTY(EditAnywhere)
	UCurveFloat* HungerSnakeTimelineCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* HungerSnakeTimelineComponent;

private:
	//Float Track Signature to handle out update track event
	FOnTimelineFloat UpdateFunctionFloat;
	FOnTimelineEvent TimelineFinishedEvent;

	//Function which updates Snake' hunger with the timeline graph
	UFUNCTION()
	void UpdateTimelineComp(float Output);
	UFUNCTION()
	void TimelineFinishedFunc();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddSnakeElement(int _ElementsNum = 1);

	void AddNewElement(int _ElementsNum = 1);

	void Move();

	void SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other);

	void SetImmortal();

	void ResetImmortal();

	void TimerResetImmortal();

	void SetBarrierCollision();

	void ResetBarrierCollision();

	void TimerResetBarrierCollisionFunc();

	void SetMovementSpeed(float Value);

	void ResetMovementSpeed();

	void TimerResetMovementSpeed();

	void PlayfromStartTimline();

	void SetHungerSnakePercent(float _HungerSnake);

	UFUNCTION(BlueprintNativeEvent)
	void AddWidgetRestartLevel();
	void AddWidgetRestartLevel_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void AddWidgetWinGame();
	void AddWidgetWinGame_Implementation();
};
