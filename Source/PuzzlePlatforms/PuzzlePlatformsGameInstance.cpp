// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance() : Super()
{

}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	//if (!WBP_MainMenu.Class)
	//	return;
	UUserWidget* mainMenu = CreateWidget<UUserWidget>(this, WBP_MainMenu);
	mainMenu->AddToViewport();
	if (APlayerController* firstPlayerController = GetFirstLocalPlayerController())
	{
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(mainMenu->TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		firstPlayerController->SetInputMode(inputMode);
		firstPlayerController->bShowMouseCursor = true;
	}
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

	if(APlayerController* firstPlayerController = GetFirstLocalPlayerController())
	{
		firstPlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}