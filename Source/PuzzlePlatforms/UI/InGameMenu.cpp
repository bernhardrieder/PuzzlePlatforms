// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Button.h"
#include "MenuInterface.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize() || !m_quitServerButton || !m_backToGameButton)
		return false;

	m_quitServerButton->OnClicked.AddDynamic(this, &UInGameMenu::quitServerBtnClicked);
	m_backToGameButton->OnClicked.AddDynamic(this, &UInGameMenu::backToGameBtnClicked);

	return true;
}

void UInGameMenu::quitServerBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->QuitServer();
	}
}

void UInGameMenu::backToGameBtnClicked()
{
	TearDown();
}
