// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance() : Super()
{

}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!WBP_MainMenu.GetDefaultObject())
		return;
	UMainMenu* mainMenu = CreateWidget<UMainMenu>(this, WBP_MainMenu);
	mainMenu->SetMenuInterface(this);
	mainMenu->Setup();
}

void UPuzzlePlatformsGameInstance::Host()
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

void UPuzzlePlatformsGameInstance::Join(const FString& address)
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
