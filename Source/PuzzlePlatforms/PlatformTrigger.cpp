// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_triggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Volume"));
	RootComponent = m_triggerVolume;
	m_triggerVolume->bGenerateOverlapEvents = true;
	m_triggerVolume->SetCollisionProfileName("OverlapOnlyPawn");
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	m_triggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::onTriggerBeginOverlap);
	m_triggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::onTriggerEndOverlap);
}

// Called every frame
void APlatformTrigger::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

void APlatformTrigger::onTriggerBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	for(int32 i = 0; i < m_affectedMovingPlatforms.Num(); ++i)
	{
		m_affectedMovingPlatforms[i]->AddActiveTrigger();
	}
}

void APlatformTrigger::onTriggerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	for (int32 i = 0; i < m_affectedMovingPlatforms.Num(); ++i)
	{
		m_affectedMovingPlatforms[i]->RemoveActiveTrigger();
	}
}

