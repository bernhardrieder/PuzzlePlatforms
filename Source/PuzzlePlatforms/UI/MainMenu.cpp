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

void UMainMenu::hostBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Host btn clicked!"));
}

void UMainMenu::joinBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("join btn clicked!"));
}
