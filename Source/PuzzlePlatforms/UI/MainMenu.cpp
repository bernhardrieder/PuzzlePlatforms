// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableTextBox.h"
#include "ServerRow.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() ||!m_hostMenuButton || !m_hostServerButton || !m_joinMenuButton || !m_joinServerButton || !m_cancelJoinMenuButton || !m_quitGameButton || !m_cancelHostMenuButton || !m_hostServerButton)
		return false;

	//main menu 
	m_hostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::hostMenuBtnClicked);
	m_joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::joinMenuBtnClicked);
	m_quitGameButton->OnClicked.AddDynamic(this, &UMainMenu::quitGameBtnClicked);

	//host menu
	m_hostServerButton->OnClicked.AddDynamic(this, &UMainMenu::hostServerBtnClicked);
	m_hostServerButton->SetIsEnabled(false);
	m_cancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::cancelHostMenuBtnClicked);
	m_serverNameTxtBox->OnTextChanged.AddDynamic(this, &UMainMenu::onServerNameTextChanged);

	//join menu
	m_joinServerButton->OnClicked.AddDynamic(this, &UMainMenu::joinServerBtnClicked);
	m_cancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::cancelJoinMenuBtnClicked);

	return true;
}

void UMainMenu::SetServerList(const TArray<FServerData>& serverData)
{
	if (!WBP_ServerRow.GetDefaultObject())
		return;

	m_serverListScrollBox->ClearChildren();
	m_selectedServerRowIndex.Reset();

	if (UWorld* const world = GetWorld())
	{
		for(int32 i = 0; i < serverData.Num(); ++i)
		{
			UServerRow* row = CreateWidget<UServerRow>(world, WBP_ServerRow);
			row->Setup(this, i);
			row->SetServerData(serverData[i]);
			m_serverListScrollBox->AddChild(row);
		}
	}
}

void UMainMenu::SelectServerFromList(uint32 index)
{
	m_selectedServerRowIndex = index;
	m_joinServerButton->SetIsEnabled(GetSelectedServerRow() ? true : false);
}

UServerRow* UMainMenu::GetSelectedServerRow() const
{
	if (m_selectedServerRowIndex.IsSet())
	{
		if (UServerRow* row = Cast<UServerRow>(m_serverListScrollBox->GetChildAt(m_selectedServerRowIndex.GetValue())))
		{
			return row;
		}
	}
	return nullptr;
}

void UMainMenu::hostMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_hostMenu);
	m_serverNameTxtBox->SetKeyboardFocus();
}

void UMainMenu::cancelHostMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_mainMenu);
	m_serverNameTxtBox->SetText(FText::FromString(""));
	m_hostMenuButton->SetKeyboardFocus();
}

void UMainMenu::hostServerBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->HostServer(m_serverNameTxtBox->GetText().ToString());
	}
}

void UMainMenu::joinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_joinMenu);
	if (m_menuInterface)
	{
		m_menuInterface->RefreshServerList();
		m_joinServerButton->SetIsEnabled(false);
	}
	m_cancelJoinMenuButton->SetKeyboardFocus();
}

void UMainMenu::joinServerBtnClicked()
{
	if (m_selectedServerRowIndex.IsSet() && m_menuInterface)
	{
		m_menuInterface->JoinServer(m_selectedServerRowIndex.GetValue());
	}
}

void UMainMenu::cancelJoinMenuBtnClicked()
{
	m_menuSwitcher->SetActiveWidget(m_mainMenu);
	m_joinMenuButton->SetKeyboardFocus();
}

void UMainMenu::quitGameBtnClicked()
{
	if (m_menuInterface)
	{
		m_menuInterface->QuitGame();
	}
}

void UMainMenu::onServerNameTextChanged(const FText& text)
{
	m_hostServerButton->SetIsEnabled(!text.IsEmpty());
}
