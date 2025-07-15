// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "BarrierBase.generated.h"

class UStaticMeshComponent;
class ASnakeBase;

UCLASS()
class SNAKEGAME_API ABarrierBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABarrierBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* MeshMaterial;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

	FTimerHandle VisibleBarrierTimerHandle;
	FTimerHandle DestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	void VisibleBarrierTimer();
	void ToggleVisBarrier();
	void DestroyTimer();
	void DestroyObj();
};
