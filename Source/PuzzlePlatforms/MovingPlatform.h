// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Moving Speed"))
	float m_movingSpeed = 10.f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Target Location", MakeEditWidget = true))
	FVector m_targetPosition;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Active Triggers Needed"))
	int32 m_activeTriggersNeeded = 1;

public:

	AMovingPlatform();
	void BeginPlay() override;
	void Tick(float deltaTime) override;
	
	void AddActiveTrigger();
	void RemoveActiveTrigger();
	bool IsMovementEnabled() const;

private:
	FVector m_globalStartLocation;
	FVector m_globalTargetLocation;
	int32 m_currentActiveTriggers = 0;
};
