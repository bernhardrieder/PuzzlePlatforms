// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !m_hostButton || !m_joinButton)
		return false;

	m_hostButton->OnClicked.AddDynamic(this, &UMainMenu::hostBtnClicked);
	m_joinButton->OnClicked.AddDynamic(this, &UMainMenu::joinBtnClicked);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* menuInterface)
{
	m_menuInterface = menuInterface;
}

void UMainMenu::hostBtnClicked()
{
	if(m_menuInterface)
	{
		m_menuInterface->Host();
	}
}

void UMainMenu::joinBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->Join();
	}
}
