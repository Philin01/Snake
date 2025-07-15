// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Components/TimelineComponent.h"
#include "FoodBase.generated.h"

//Declare our delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyDelegate);

class ASnakeBase;
class UStaticMeshComponent;

UCLASS()
class SNAKEGAME_API AFoodBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFoodBase();

	//Food variables
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* FoodMaterial;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 15.f;

	//Timeline variables
	UPROPERTY(EditAnywhere)
	UCurveFloat* RotationCurve;

	FTimerHandle VisibleTimerHandle;
	FTimerHandle DestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* SetRotationTimelineComponent;

private:
	FOnTimelineFloat UpdateFuncFloat;
	FOnTimelineEvent TimelineFinishedEvent;

	UFUNCTION()
	void UpdateFunc(float Output);
	UFUNCTION()
	void FinishedFunc();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;

	FMyDelegate SpawnFood;

	void VisibleTimer();
	void ToggleVis();
	void DestroyFood();
	void DestroyFoodTimer();
};
