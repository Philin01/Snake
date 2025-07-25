// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "SnakeElementBase.generated.h"

class UStaticMeshComponent;
class ASnakeBase;

UCLASS()
class SNAKEGAME_API ASnakeElementBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ASnakeElementBase();

	//SnakeElement's variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* SnakeHeadMaterial = nullptr;
	//Snakes variables
	UPROPERTY()
	ASnakeBase* SnakeOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void SetFirstElemType();
	void SetFirstElemType_Implementation();

	UFUNCTION()
	void HandleBeginOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void ToggleCollision();
	void BarrierCollisionDisabled();
	void BarrierCollisionEnabled();

	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
