// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
class UServerRow;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()
	
	FString Name;
	uint32 ConnectedPlayers;
	uint32 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* m_hostMenuButton = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UButton* m_joinMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_joinServerButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_cancelJoinMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_quitGameButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* m_menuSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* m_mainMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* m_joinMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget* m_hostMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* m_serverListScrollBox = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerRow> WBP_ServerRow;

	UPROPERTY(meta =(BindWidget))
	UEditableTextBox* m_serverNameTxtBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_cancelHostMenuButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_hostServerButton = nullptr;

public:
	virtual bool Initialize() override;
	void SetServerList(const TArray<FServerData>& serverData);
	void SelectServerFromList(uint32 index);
	UServerRow* GetSelectedServerRow() const;

private:
	UFUNCTION()
	void hostMenuBtnClicked();

	UFUNCTION()
	void cancelHostMenuBtnClicked();

	UFUNCTION()
	void hostServerBtnClicked();

	UFUNCTION()
	void joinMenuBtnClicked();

	UFUNCTION()
	void joinServerBtnClicked();

	UFUNCTION()
	void cancelJoinMenuBtnClicked();

	UFUNCTION()
	void quitGameBtnClicked();

	UFUNCTION()
	void onServerNameTextChanged(const FText& text);

	TOptional<uint32> m_selectedServerRowIndex;
};
