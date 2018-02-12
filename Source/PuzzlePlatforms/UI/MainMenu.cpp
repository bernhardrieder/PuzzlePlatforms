// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableTextBox.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !m_hostServerButton || !m_joinMenuButton || !m_joinServerButton || !m_cancelJoinMenuButton)
		return false;

	m_hostServerButton->OnClicked.AddDynamic(this, &UMainMenu::hostServerBtnClicked);
	m_joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::joinMenuBtnClicked);
	m_joinServerButton->OnClicked.AddDynamic(this, &UMainMenu::joinServerBtnClicked);
	m_cancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::cancelJoinMenuBtnClicked);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* menuInterface)
{
	m_menuInterface = menuInterface;
}

void UMainMenu::Setup()
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

void UMainMenu::hostServerBtnClicked()
{
	if(m_menuInterface)
	{
		m_menuInterface->HostServer();
	}
}

void UMainMenu::joinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_joinMenu);
}

void UMainMenu::joinServerBtnClicked()
{
	FString ipAddress = m_ipAddressTxtBox->GetText().ToString();
	if (m_menuInterface && !ipAddress.IsEmpty())
	{
		m_menuInterface->JoinServer(ipAddress);
	}
}

void UMainMenu::cancelJoinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_mainMenu);
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* inLevel, UWorld* inWorld)
{
	Super::OnLevelRemovedFromWorld(inLevel, inWorld);
	if (APlayerController* firstPlayerController = inWorld->GetFirstPlayerController())
	{
		firstPlayerController->SetInputMode(FInputModeGameOnly());
		firstPlayerController->bShowMouseCursor = false;
	}
}
