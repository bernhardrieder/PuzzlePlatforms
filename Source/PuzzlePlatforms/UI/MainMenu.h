// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* m_hostButton = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UButton* m_joinButton = nullptr;

public:
	virtual bool Initialize() override;

private:

	UFUNCTION()
	void hostBtnClicked();

	UFUNCTION()
	void joinBtnClicked();
};
