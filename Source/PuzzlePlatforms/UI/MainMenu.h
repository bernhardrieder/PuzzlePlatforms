// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* m_hostServerButton = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UButton* m_joinMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_joinServerButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_cancelJoinMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* m_menuSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* m_mainMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* m_joinMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* m_ipAddressTxtBox = nullptr;

public:
	virtual bool Initialize() override;
	void Setup();
	void SetMenuInterface(IMenuInterface* menuInterface);

private:

	UFUNCTION()
	void hostServerBtnClicked();

	UFUNCTION()
	void joinMenuBtnClicked();

	UFUNCTION()
	void joinServerBtnClicked();

	UFUNCTION()
	void cancelJoinMenuBtnClicked();

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* inLevel, UWorld* inWorld) override;

private:
	IMenuInterface* m_menuInterface;
};
