// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

ALobbyGameMode::ALobbyGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
	++m_playerCount;
	if (m_playerCount == m_numOfPlayerNeededToLeaveTheLobby)
	{
		m_currentWaitSecondsLeft = m_secondsToWaitForJoingingOtherPlayersBeforeTravelling;
		GetWorldTimerManager().SetTimer(m_waitBeforeTravellingTimerHandle, this, &ALobbyGameMode::timerWaitRepeatBeforeTravelling, 1.0f, true);
	}
}

void ALobbyGameMode::Logout(AController* exiting)
{
	Super::Logout(exiting);
	--m_playerCount;
	if (m_playerCount < m_numOfPlayerNeededToLeaveTheLobby)
	{
		GetWorldTimerManager().ClearTimer(m_waitBeforeTravellingTimerHandle);
	}
}

void ALobbyGameMode::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	if (GEngine)
	{
		if (m_playerCount < m_numOfPlayerNeededToLeaveTheLobby)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Green, FString::Printf(TEXT("Not enough players to start the game! Players needed: %i"), m_numOfPlayerNeededToLeaveTheLobby));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Green, FString::Printf(TEXT("Time left before starting game: %i seconds"), m_currentWaitSecondsLeft));
		}
	}
}

void ALobbyGameMode::timerWaitRepeatBeforeTravelling()
{
	if (--m_currentWaitSecondsLeft < 0)
	{
		GetWorldTimerManager().ClearTimer(m_waitBeforeTravellingTimerHandle);
		travelAllPlayersToTheFirstLevel();
	}
}

void ALobbyGameMode::travelAllPlayersToTheFirstLevel()
{
	if (m_firstPuzzleLevel.IsNull())
	{
		const FString errorMessage = "there is no fist puzzle level!";
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Red, errorMessage);
		else
		UE_LOG(LogTemp, Error, TEXT("%s"), *errorMessage);

		return;
	}

	if (UWorld* const world = GetWorld())
	{
		if (UPuzzlePlatformsGameInstance* gameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance()))
		{
			gameInstance->StartSession();
		}
		bUseSeamlessTravel = true;
		world->ServerTravel(m_firstPuzzleLevel.GetLongPackageName().Append("?listen"));
	}
}
