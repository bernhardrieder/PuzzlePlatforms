// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Num of player needed to leave the lobby"))
	int32 m_numOfPlayerNeededToLeaveTheLobby = 2;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Seconds to wait for joining other players before travelling"))
	int32 m_secondsToWaitForJoingingOtherPlayersBeforeTravelling = 5;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Fist Puzzle Level"))
	TAssetPtr<UWorld> m_firstPuzzleLevel;

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* exiting) override;
	virtual void Tick(float deltaSeconds) override;

private:
	void timerWaitRepeatBeforeTravelling();
	void travelAllPlayersToTheFirstLevel();

private:
	int32 m_playerCount = 0;
	int32 m_currentWaitSecondsLeft = 0;
	FTimerHandle m_waitBeforeTravellingTimerHandle;
};
