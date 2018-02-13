// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"
#include "UI/InGameMenu.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance() : Super()
{
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	m_onlineSubsystem = IOnlineSubsystem::Get();
	if (m_onlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("found online subsystem with name %s"), *m_onlineSubsystem->GetSubsystemName().ToString());
		m_sessionInterface = m_onlineSubsystem->GetSessionInterface();
		if (m_sessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("session interface available"));
			m_sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onCreateSessionCompleted);
			m_sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onDestroySessionCompleted);
			m_sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onFindSessionsCompleted);
			m_sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onJoinSessionCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("didn't found an online subsystem"))
	}
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!WBP_MainMenu.GetDefaultObject())
		return;
	m_mainMenu = CreateWidget<UMainMenu>(this, WBP_MainMenu);
	m_mainMenu->SetMenuInterface(this);
	m_mainMenu->Setup();
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!WBP_InGameMenu.GetDefaultObject())
		return;
	UMenuWidget* menu = CreateWidget<UMenuWidget>(this, WBP_InGameMenu);
	menu->SetMenuInterface(this);
	menu->Setup();
}

void UPuzzlePlatformsGameInstance::HostServer()
{
	if (LevelToHost.IsNull())
	{
		const FString errorMessage = "there is no level to host!";
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Red, errorMessage);
		else
			UE_LOG(LogTemp, Error, TEXT("%s"), *errorMessage);

		return;
	}

	if (m_sessionInterface.IsValid())
	{
		if(m_sessionInterface->GetNamedSession(m_sessionName))
		{
			m_sessionInterface->DestroySession(m_sessionName);
		}
		else
		{
			createNewSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::onCreateSessionCompleted(FName sessionName, bool success)
{
	if(success)
	{
		UE_LOG(LogTemp, Warning, TEXT("session \"%s\" created"), *sessionName.ToString());

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString("Hosting"));
		}
		if (UWorld* const world = GetWorld())
		{
			world->ServerTravel(LevelToHost.GetLongPackageName().Append("?listen"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("can't create session \"%s\""), *sessionName.ToString());
	}
}

void UPuzzlePlatformsGameInstance::onDestroySessionCompleted(FName sessionName, bool success)
{
	if(success)
	{
		createNewSession();
	}
}

void UPuzzlePlatformsGameInstance::onFindSessionsCompleted(bool success)
{
	if(success && m_sessionSearch.IsValid() && m_mainMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %i sessions"), m_sessionSearch->SearchResults.Num());

		TArray<FString> foundServerNames;
		if(m_sessionSearch->SearchResults.Num() > 0 )
		{
			for (auto& result : m_sessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *result.GetSessionIdStr());
				foundServerNames.Add(result.GetSessionIdStr());
			}
		}
		m_mainMenu->SetServerList(foundServerNames);
	}
}

void UPuzzlePlatformsGameInstance::createNewSession()
{
	if (m_sessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = true;
		sessionSettings.NumPublicConnections = 2;
		sessionSettings.bShouldAdvertise = true;
		m_sessionInterface->CreateSession(0, m_sessionName, sessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::onJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type bWasSuccessful)
{
	if (!bWasSuccessful || !m_sessionInterface.IsValid())
		return;

	FString address;
	if(!m_sessionInterface->GetResolvedConnectString(sessionName, address))
	{
		const FString errorMessage = "couldn't resolve connect string!!!";
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Red, errorMessage);
		else
			UE_LOG(LogTemp, Error, TEXT("%s"), *errorMessage);

		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *address));
	}

	if (APlayerController* firstPlayerController = GetFirstLocalPlayerController())
	{
		firstPlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::JoinServer(int32 serverIndex)
{
	if (!m_sessionInterface.IsValid() || !m_sessionSearch.IsValid())
		return;

	m_sessionInterface->JoinSession(0, m_sessionName, m_sessionSearch->SearchResults[serverIndex]);
}

void UPuzzlePlatformsGameInstance::QuitServer()
{
	if (UWorld* world = GetWorld())
	{
		if (world->IsServer())
		{
			world->GetAuthGameMode()->ReturnToMainMenuHost();
		}
		else
		{
			if (APlayerController* firstPlayerController = GetFirstLocalPlayerController())
			{
				firstPlayerController->ClientReturnToMainMenu("Back to main menu");
			}
		}
	}
}

void UPuzzlePlatformsGameInstance::QuitGame()
{
	//in case this happens during the game
	if (UWorld* world = GetWorld())
	{
		if (world->IsServer())
		{
			world->GetAuthGameMode()->ReturnToMainMenuHost();
		}
	}

	if (APlayerController* firstPlayerController = GetFirstLocalPlayerController())
	{
		UKismetSystemLibrary::QuitGame(firstPlayerController, firstPlayerController, EQuitPreference::Quit);
	}
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	if (m_sessionInterface.IsValid())
	{
		m_sessionSearch = MakeShareable<FOnlineSessionSearch>(new FOnlineSessionSearch());
		if (m_sessionSearch.IsValid())
		{
			//m_sessionSearch->bIsLanQuery = true;
			m_sessionInterface->FindSessions(0, m_sessionSearch.ToSharedRef());
		}
	}
}
