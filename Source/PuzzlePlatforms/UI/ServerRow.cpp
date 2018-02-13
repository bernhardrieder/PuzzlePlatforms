// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "TextBlock.h"
#include "Button.h"
#include "MainMenu.h"
#include "Image.h"

bool UServerRow::Initialize()
{
	if (!Super::Initialize() || !m_selectButton || !m_isSelectedBackground)
		return false;

	m_isSelectedBackground->SetVisibility(ESlateVisibility::Hidden);
	m_selectButton->OnClicked.AddDynamic(this, &UServerRow::onSelectBtnClicked);

	return true;
}

void UServerRow::SetServerName(const FText& serverName)
{
	m_serverName = serverName;
	m_serverNameTxt->SetText(serverName);
}

void UServerRow::Setup(UMainMenu* parent, int32 rowIndex)
{
	m_mainMenu = parent;
	m_rowIndex = rowIndex;
}

void UServerRow::onSelectBtnClicked()
{
	if(m_mainMenu)
	{
		if (UServerRow* previousSelectedRow = m_mainMenu->GetSelectedServerRow())
		{
			previousSelectedRow->m_isSelectedBackground->SetVisibility(ESlateVisibility::Hidden);
		}
		m_isSelectedBackground->SetVisibility(ESlateVisibility::Visible);
		m_mainMenu->SelectServerFromList(m_rowIndex);
	}
}
