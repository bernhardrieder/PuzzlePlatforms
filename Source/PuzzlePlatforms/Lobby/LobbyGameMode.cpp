// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

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
	if(m_playerCount == m_numOfPlayerNeededToLeaveTheLobby)
	{
		travelAllPlayersToTheFirstLevel();
	}
}

void ALobbyGameMode::Logout(AController* exiting)
{
	Super::Logout(exiting);
	--m_playerCount;
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

	if(UWorld* const world = GetWorld())
	{
		bUseSeamlessTravel = true;
		world->ServerTravel(m_firstPuzzleLevel.GetLongPackageName().Append("?listen"));
	}
}
