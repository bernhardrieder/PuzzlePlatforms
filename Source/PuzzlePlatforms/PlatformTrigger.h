// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

class UBoxComponent;
class AMovingPlatform;

UCLASS()
class PUZZLEPLATFORMS_API APlatformTrigger : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Trigger Volume"))
	UBoxComponent* m_triggerVolume = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Affected Moving Platforms"))
	TArray<AMovingPlatform*> m_affectedMovingPlatforms;

public:	
	// Sets default values for this actor's properties
	APlatformTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float deltaTime) override;

private:
	UFUNCTION()
	void onTriggerBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult);
	
	UFUNCTION()
	void onTriggerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
