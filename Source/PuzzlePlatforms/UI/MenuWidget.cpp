// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "MenuInterface.h"

void UMenuWidget::Setup()
{
	this->AddToViewport();

	if (UWorld* const world = GetWorld())
	{
		if (APlayerController* firstPlayerController = world->GetFirstPlayerController())
		{
			FInputModeUIOnly inputMode;
			inputMode.SetWidgetToFocus(this->TakeWidget());
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			firstPlayerController->SetInputMode(inputMode);
			firstPlayerController->bShowMouseCursor = true;
		}
	}
}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();
	if(UWorld* world = GetWorld())
	{
		if (APlayerController* firstPlayerController = world->GetFirstPlayerController())
		{
			firstPlayerController->SetInputMode(FInputModeGameOnly());
			firstPlayerController->bShowMouseCursor = false;
		}
	}
}

void UMenuWidget::SetMenuInterface(IMenuInterface* menuInterface)
{
	m_menuInterface = menuInterface;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* inLevel, UWorld* inWorld)
{
	Super::OnLevelRemovedFromWorld(inLevel, inWorld);
	TearDown();
}
