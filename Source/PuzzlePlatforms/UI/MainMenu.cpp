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

void UMainMenu::SetServerList(const TArray<FString>& serverNames)
{
	if (!WBP_ServerRow.GetDefaultObject())
		return;

	m_serverListScrollBox->ClearChildren();
	m_selectedServerRowIndex.Reset();

	if (UWorld* const world = GetWorld())
	{
		for(int32 i = 0; i < serverNames.Num(); ++i)
		{
			UServerRow* row = CreateWidget<UServerRow>(world, WBP_ServerRow);
			row->Setup(this, i);
			row->SetServerName(FText::FromString(serverNames[i]));
			m_serverListScrollBox->AddChild(row);
		}
	}
}

void UMainMenu::SelectServerFromList(uint32 index)
{
	m_selectedServerRowIndex = index;
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
	if(UServerRow* row = GetSelectedServerRow())
	{
		if (m_menuInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("i want to join server %s"), *row->GetServerName().ToString());
			//	m_menuInterface->JoinServer(ipAddress);
		}
	}
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
