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
		IOnlineSessionPtr sessionInterface = m_onlineSubsystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("session interface available"));
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onCreateSessionCompleted);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onDestroySessionCompleted);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::onFindSessionsCompleted);

			m_sessionSearch = MakeShareable<FOnlineSessionSearch>(new FOnlineSessionSearch());
			if(m_sessionSearch.IsValid())
			{
				sessionInterface->FindSessions(0, m_sessionSearch.ToSharedRef());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("didn't found a online subsystem"))
	}
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!WBP_MainMenu.GetDefaultObject())
		return;
	UMenuWidget* menu = CreateWidget<UMenuWidget>(this, WBP_MainMenu);
	menu->SetMenuInterface(this);
	menu->Setup();
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

	IOnlineSessionPtr sessionInterface = m_onlineSubsystem->GetSessionInterface();
	if (sessionInterface.IsValid())
	{
		if(sessionInterface->GetNamedSession(m_sessionName))
		{
			sessionInterface->DestroySession(m_sessionName);
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
	if(success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %i sessions"), m_sessionSearch.Get()->SearchResults.Num());

		if(m_sessionSearch.Get()->SearchResults.Num() > 0 )
		{
			UE_LOG(LogTemp, Warning, TEXT("Sessions: "));
			for (auto& result : m_sessionSearch.Get()->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *result.GetSessionIdStr());
			}
		}
	}
}

void UPuzzlePlatformsGameInstance::createNewSession()
{
	IOnlineSessionPtr sessionInterface = m_onlineSubsystem->GetSessionInterface();
	if (sessionInterface.IsValid())
	{
		sessionInterface->CreateSession(0, m_sessionName, FOnlineSessionSettings());
	}
}

void UPuzzlePlatformsGameInstance::JoinServer(const FString& address)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *address));
	}

	if (APlayerController* firstPlayerController = GetFirstLocalPlayerController())
	{
		firstPlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
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

