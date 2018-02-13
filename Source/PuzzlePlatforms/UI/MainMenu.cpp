// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableTextBox.h"
#include "ServerRow.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !m_hostServerButton || !m_joinMenuButton || !m_joinServerButton || !m_cancelJoinMenuButton || !m_quitGameButton)
		return false;

	m_hostServerButton->OnClicked.AddDynamic(this, &UMainMenu::hostServerBtnClicked);
	m_joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::joinMenuBtnClicked);
	m_joinServerButton->OnClicked.AddDynamic(this, &UMainMenu::joinServerBtnClicked);
	m_cancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::cancelJoinMenuBtnClicked);
	m_quitGameButton->OnClicked.AddDynamic(this, &UMainMenu::quitGameBtnClicked);

	return true;
}

void UMainMenu::SetServerList(const TArray<FString> serverNames)
{
	if (!WBP_ServerRow.GetDefaultObject())
		return;

	m_serverListScrollBox->ClearChildren();

	if (UWorld* const world = GetWorld())
	{
		for (const FString& serverName : serverNames)
		{
			UServerRow* row = CreateWidget<UServerRow>(world, WBP_ServerRow);
			row->SetServerName(FText::FromString(serverName));
			m_serverListScrollBox->AddChild(row);
		}
	}
}

void UMainMenu::hostServerBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->HostServer();
	}
}

void UMainMenu::joinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_joinMenu);
	if (m_menuInterface)
	{
		m_menuInterface->RefreshServerList();
	}
}

void UMainMenu::joinServerBtnClicked()
{
	//FString ipAddress = m_ipAddressTxtBox->GetText().ToString();
	//if (m_menuInterface && !ipAddress.IsEmpty())
	//{
	//	m_menuInterface->JoinServer(ipAddress);
	//}
}

void UMainMenu::cancelJoinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_mainMenu);
}

void UMainMenu::quitGameBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->QuitGame();
	}
}
