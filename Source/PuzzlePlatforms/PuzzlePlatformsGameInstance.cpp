// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"
#include "UI/InGameMenu.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystem.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance() : Super()
{

}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	if(onlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("found online subsystem with name %s"), *onlineSubsystem->GetSubsystemName().ToString())
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
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString("Hosting"));
	}
	if(UWorld* const world = GetWorld())
	{
		world->ServerTravel(LevelToHost.GetLongPackageName().Append("?listen"));
	}
}

void UPuzzlePlatformsGameInstance::JoinServer(const FString& address)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *address));
	}

	if(APlayerController* firstPlayerController = GetFirstLocalPlayerController())
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
