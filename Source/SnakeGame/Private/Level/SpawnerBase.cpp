// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/SpawnerBase.h"
#include "Level/FloorBlockBase.h"
#include "Game/Interact/FoodBase.h"
#include "Game/Character/SnakeElementBase.h"
#include "Game/Character/SnakeBase.h"
#include "Level/BarrierBase.h"
#include "Level/FakeBarrier.h"
#include "Game/Interact/FoodSpeedUpBonus.h"
#include "Game/Interact/FoodSpeedDownDebuf.h"
#include "Game/Interact/FoodBarrierIgnoreBonus.h"
#include "Game/Interact/DoubleFoodBonus.h"
#include "Level/WallBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/OverlapResult.h"
#include <cmath>

// Sets default values
ASpawnerBase::ASpawnerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	X = 10;
	Y = 10;
	Distance = 50;
	DefaultBarrierNum = 5;
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnFloorGrid(X, Y, Distance);
	RandomSpawnFood();
	SpawnBarrierTimer();
	SpawnWall(25.f, -850.f, 25.f, 0.f, 0.f, 0.f, 9.f, 0.5f, 2.f);
	SpawnWall(500.f, 0.f, 25.f, 0.f, 0.f, 90.f, 17.5f, 0.5f, 2.f);
	SpawnWall(25.f, 850.f, 25.f, 0.f, 0.f, 0.f, 9.f, 0.5f, 2.f);
	SpawnWall(-450.f, 0.f, -30.f, 0.f, 0.f, 90.f, 17.5f, 0.5f, 1.f);
}

// Called every frame
void ASpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnerBase::SpawnFloorGrid(int _X, int _Y, float _DIstance)
{
	for (int i = 0; i < _X; ++i)
	{
		auto StartXPosition = -400;
		auto NewXPosition = i * Distance;
		auto CurrentXPosition = StartXPosition + NewXPosition;
		for (int j = 0; j < _Y; ++j)
		{
			auto StartYPosition = -800;
			auto NewYPosition = j * Distance;
			auto CurrentYPosition = StartYPosition + NewYPosition;
			FVector NewLocation(CurrentXPosition, CurrentYPosition, -50);
			FTransform NewTransform(NewLocation);
			int BlockIndex = i + j;
			AFloorBlockBase* NewFirstBlock = GetWorld()->SpawnActor<AFloorBlockBase>(BlockClass, NewTransform);
			NewFirstBlock->SetBlockMaterial(BlockIndex);
			FloorGrid.Add(NewFirstBlock);
		}
	}
}

void ASpawnerBase::SpawnWall
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
)
{
	FVector NewLocation(XLocation, YLocation, ZLocation);
	FRotator NewRotation(PitchRotation, YawRotation, RollRotation);
	FVector NewScale(XScale, YScale, ZScale);
	FTransform NewTransform(NewRotation, NewLocation, NewScale);
	AWallBase* NewWall = GetWorld()->SpawnActor<AWallBase>(WallClass, NewTransform);
}

void ASpawnerBase::RandomSpawnFood()
{
	if (FloorGrid.Num())
	{
		FActorSpawnParameters SpawnParams;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FVector SpawnLocation;
		FVector ZLocation(0, 0, 51);
		bool bLocationFound = false;

		float SphereRadius = 50.0f;

		while (!bLocationFound && FloorGrid.Num())
		{
			//Выбираю рандомный индекс из массива и узнаю его локацию
			int32 RandomIndex = FMath::RandRange(0, FloorGrid.Num() - 1);
			SpawnLocation = FloorGrid[RandomIndex]->GetActorLocation();

			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			bool bOverlap = GetWorld()->OverlapMultiByObjectType
			(
				OverlapResults,
				SpawnLocation + ZLocation,
				FQuat::Identity,
				FCollisionObjectQueryParams(ECC_OverlapAll_Deprecated),
				FCollisionShape::MakeSphere(SphereRadius),
				CollisionParams
			);

			bool bSpecificActorFound = false;
			for (const FOverlapResult& Result : OverlapResults)
			{
				if (Result.GetActor() && Result.GetActor()->IsA(ASnakeElementBase::StaticClass()) ||
					Result.GetActor() && Result.GetActor()->IsA(ABarrierBase::StaticClass()))
				{
					bSpecificActorFound = true;
					break;
				}
			}

			if (bOverlap == false || bSpecificActorFound == false)
			{
				bLocationFound = true;
			}
		}

		if (bLocationFound)
		{
			if (UKismetMathLibrary::RandomBoolWithWeight(RandSpeedUpBonusValue))
			{
				AFoodBase* CurrentFood = GetWorld()->SpawnActor<AFoodBase>
					(
						FoodSpeedUpBonusClass,
						SpawnLocation + ZLocation,
						SpawnRotation,
						SpawnParams
					);
				if (CurrentFood)
				{
					CurrentFood->SpawnFood.AddDynamic(this, &ASpawnerBase::RandomSpawnFood);
				}
			}
			else if (UKismetMathLibrary::RandomBoolWithWeight(RandSpeedDownDebufValue))
			{
				AFoodBase* CurrentFood = GetWorld()->SpawnActor<AFoodBase>
					(
						FoodSpeedDownDebufClass,
						SpawnLocation + ZLocation,
						SpawnRotation,
						SpawnParams
					);
				if (CurrentFood)
				{
					CurrentFood->SpawnFood.AddDynamic(this, &ASpawnerBase::RandomSpawnFood);
				}
			}
			else if (UKismetMathLibrary::RandomBoolWithWeight(RandBarrierIgnoreBonusValue))
			{
				AFoodBase* CurrentFood = GetWorld()->SpawnActor<AFoodBase>
					(
						FoodBarrierIgnoreBonusClass,
						SpawnLocation + ZLocation,
						SpawnRotation,
						SpawnParams
					);
				if (CurrentFood)
				{
					CurrentFood->SpawnFood.AddDynamic(this, &ASpawnerBase::RandomSpawnFood);
				}
			}
			else if (UKismetMathLibrary::RandomBoolWithWeight(RandDoubleFoodBonusValue))
			{
				AFoodBase* CurrentFood = GetWorld()->SpawnActor<AFoodBase>
					(
						DoubleFoodBonusClass,
						SpawnLocation + ZLocation,
						SpawnRotation,
						SpawnParams
					);
				if (CurrentFood)
				{
					CurrentFood->SpawnFood.AddDynamic(this, &ASpawnerBase::RandomSpawnFood);
				}
			}
			else
			{
				AFoodBase* CurrentFood = GetWorld()->SpawnActor<AFoodBase>
					(
						FoodClass,
						SpawnLocation + ZLocation,
						SpawnRotation,
						SpawnParams
					);
				if (CurrentFood)
				{
					CurrentFood->SpawnFood.AddDynamic(this, &ASpawnerBase::RandomSpawnFood);
				}
			}
		}
		else
		{
			FVector Center = SpawnLocation + ZLocation;
			float Radius = SphereRadius;
			int32 Segments = 12;
			FColor Color = FColor::Green;
			float LifeTime = 5.0f;
			float Thickness = 1.0f;

			UKismetSystemLibrary::DrawDebugSphere
			(
				GetWorld(),
				Center,
				Radius,
				Segments,
				Color,
				LifeTime,
				Thickness
			);
		}
	}
}

void ASpawnerBase::RandomSpawnBarriers(int _BarrierNum)
{
	if (FloorGrid.Num())
	{
		for (int i = 0; i < _BarrierNum; ++i)
		{
			FActorSpawnParameters SpawnParams;
			FRotator SpawnRotation = FRotator::ZeroRotator;
			FVector SpawnLocation;
			FVector ZLocation(0, 0, 50);
			bool bLocationFound = false;

			FVector BoxExtend(100.f, 100.f, 49.f);

			while (!bLocationFound && FloorGrid.Num())
			{
				//Выбираю рандомный индекс из массива и узнаю его локацию
				int32 RandomIndex = FMath::RandRange(0, FloorGrid.Num() - 1);
				SpawnLocation = FloorGrid[RandomIndex]->GetActorLocation();

				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);
				CollisionParams.AddIgnoredActor(BarrierPtr);

				bool bOverlap = GetWorld()->OverlapMultiByObjectType
				(
					OverlapResults,
					SpawnLocation + ZLocation,
					FQuat::Identity,
					FCollisionObjectQueryParams(ECC_OverlapAll_Deprecated),
					FCollisionShape::MakeBox(BoxExtend),
					CollisionParams
				);

				bool bSpecificActorFound = false;
				for (const FOverlapResult& Result : OverlapResults)
				{
					if (Result.GetActor() && Result.GetActor()->IsA(ASnakeElementBase::StaticClass()) ||
						Result.GetActor() && Result.GetActor()->IsA(AFoodBase::StaticClass()))
					{
						bSpecificActorFound = true;
						break;
					}
				}

				if (!bOverlap || !bSpecificActorFound)
				{
					bLocationFound = true;
				}
			}


			if (bLocationFound)
			{
				if (UKismetMathLibrary::RandomBoolWithWeight(RandSpawnFakeBarrierValue))
				{
					ABarrierBase* NewFakeBarrier = GetWorld()->SpawnActor<ABarrierBase>
						(
							FakeBarrierClass,
							SpawnLocation + ZLocation,
							SpawnRotation,
							SpawnParams
						);
				}
				else
				{
					ABarrierBase* NewBarrier = GetWorld()->SpawnActor<ABarrierBase>
						(
							BarrierClass,
							SpawnLocation + ZLocation,
							SpawnRotation,
							SpawnParams
						);
				}				
			}
			else
			{
				FVector Center = SpawnLocation + ZLocation;
				FVector Extend = BoxExtend;
				int32 Segments = 12;
				FColor Color = FColor::Green;
				float Duration = 5.0f;
				float Thickness = 1.0f;

				UKismetSystemLibrary::DrawDebugBox
				(
					GetWorld(),
					Center,
					BoxExtend,
					Color,
					FRotator::ZeroRotator,
					Duration,
					Thickness
				);
			}
		}
	}
}

void ASpawnerBase::SpawnBarrierTimer()
{
		GetWorld()->GetTimerManager().SetTimer
		(
			SpawnBarrierTimerHandle,
			this,
			&ASpawnerBase::SpawnBarriers,
			10.f,
			true,
			0.5f
		);
}

void ASpawnerBase::SpawnBarriers()
{
	//The number of snake elements is compared with the floor grid and spawn the specified number of barriers
	int NextBarrierNum = DefaultBarrierNum - SubtractionBarrierValue;
	int NextGridValue = round((FloorGrid.Num() * NextBarrierNum) / DefaultBarrierNum);
	ASnakeBase* Snake = Cast<ASnakeBase>(UGameplayStatics::GetActorOfClass(this, SnakeClass));

	if (IsValid(Snake))
	{
		if (Snake->SnakeElements.Num() >= FloorGrid.Num() - NextGridValue)
		{
			SubtractionBarrierValue += 1;
			RandomSpawnBarriers(NextBarrierNum);
		}
		else if (NextBarrierNum == DefaultBarrierNum - (DefaultBarrierNum - 1))
		{
			RandomSpawnBarriers(0);
		}
		else
		{
			RandomSpawnBarriers(DefaultBarrierNum);
		}
	}
}

