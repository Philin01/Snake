// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorBlockBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API AFloorBlockBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFloorBlockBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* FirstBlockMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SecondBlockMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBlockMaterial(int _BlockIndex);
};
