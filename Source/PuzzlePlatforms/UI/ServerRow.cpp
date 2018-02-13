// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "TextBlock.h"

void UServerRow::SetServerName(const FText& serverName)
{
	m_serverName->SetText(serverName);
}
