// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerBase.generated.h"

class AFloorBlockBase;
class AFoodBase;
class ABarrierBase;
class ASnakeElementBase;
class ASnakeBase;
class AFoodSpeedUpBonus;
class AFoodSpeedDownDebuf;
class AFoodBarrierIgnoreBonus;
class ADoubleFoodBonus;
class AFakeBarrier;
class AWallBase;

UCLASS()
class SNAKEGAME_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerBase();

	//FloorGrid variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloorBlockBase> BlockClass;
	UPROPERTY()
	TArray<AFloorBlockBase*> FloorGrid;
	UPROPERTY()
	AFloorBlockBase* FloorBlock;
	UPROPERTY(EditDefaultsOnly)
	int X;
	UPROPERTY(EditDefaultsOnly)
	int Y;
	UPROPERTY(EditDefaultsOnly)
	float Distance;

	//Wall's variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallBase> WallClass;

	//Food variebles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFoodBase> FoodClass;

	//Food bonus or debuf variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFoodSpeedUpBonus> FoodSpeedUpBonusClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFoodSpeedDownDebuf> FoodSpeedDownDebufClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFoodBarrierIgnoreBonus> FoodBarrierIgnoreBonusClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoubleFoodBonus> DoubleFoodBonusClass;
	UPROPERTY(EditDefaultsOnly)
	float RandSpeedUpBonusValue;
	UPROPERTY(EditDefaultsOnly)
	float RandSpeedDownDebufValue;
	UPROPERTY(EditDefaultsOnly)
	float RandBarrierIgnoreBonusValue;
	UPROPERTY(EditDefaultsOnly)
	float RandDoubleFoodBonusValue;

	//Barrier variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABarrierBase> BarrierClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFakeBarrier> FakeBarrierClass;
	UPROPERTY()
	ABarrierBase* BarrierPtr;
	UPROPERTY()
	AFakeBarrier* FakeBarrierPtr;
	UPROPERTY()
	TArray<ABarrierBase*> BarriersArray;
	UPROPERTY(EditDefaultsOnly)
	int DefaultBarrierNum;
	FTimerHandle SpawnBarrierTimerHandle;
	UPROPERTY(EditDefaultsOnly)
	float RandSpawnFakeBarrierValue;
	int SubtractionBarrierValue = 1;

	//Snake variables
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBase> SnakeClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnFloorGrid(int _X, int _Y, float _DIstance);
	void SpawnWall
	(
		float XLocation,
		float YLocation,
		float ZLocation,
		float RollRotation,
		float PitchRotation,
		float YawRotation,
		float XScale,
		float YScale,
		float ZScale
	);

	UFUNCTION()
	void RandomSpawnFood();
	void RandomSpawnBarriers(int _BarrierNum);

	void SpawnBarrierTimer();
	void SpawnBarriers();
};
