// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance() : Super()
{

}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
}

void UPuzzlePlatformsGameInstance::Host()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString("Hosting"));
	}
	if(UWorld* const world = GetWorld())
	{
		world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& address)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *address));
	}

	APlayerController* firstPlayerController = GetFirstLocalPlayerController();
	firstPlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}
