// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	m_globalStartLocation = GetActorLocation();
	m_globalTargetLocation = GetTransform().TransformPosition(m_targetPosition);

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority() && IsMovementEnabled())
	{
		const float journeyLength = (m_globalTargetLocation - m_globalStartLocation).Size();
		const float journeyTravelled = (GetActorLocation() - m_globalStartLocation).Size();
		if(journeyTravelled >= journeyLength)
		{
			const FVector tmp = m_globalStartLocation;
			m_globalStartLocation = m_globalTargetLocation;
			m_globalTargetLocation = tmp;
		}
		const FVector moveDirection = (m_globalTargetLocation - m_globalStartLocation).GetSafeNormal();
		const FVector movementDelta = m_movingSpeed * deltaTime * moveDirection;
		SetActorLocation(GetActorLocation() + movementDelta);
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	++m_currentActiveTriggers;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(m_currentActiveTriggers > 0)
	{
		--m_currentActiveTriggers;
	}
}

bool AMovingPlatform::IsMovementEnabled() const
{
	return m_currentActiveTriggers >= m_activeTriggersNeeded;
}
