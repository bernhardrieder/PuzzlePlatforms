// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

struct FServerData;
class UTextBlock;
class UMainMenu;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* m_selectButton = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_serverNameTxt = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_playerStatsTxt = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* m_isSelectedBackground = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_hostNameTxt = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	bool bIsHeader = false;

public:
	virtual bool Initialize() override;
	void SetServerData(const FServerData& serverData);
	void Setup(UMainMenu* parent, int32 rowIndex);

private:
	UFUNCTION()
	void onSelectBtnClicked();

private:
	UMainMenu* m_mainMenu;
	int32 m_rowIndex;
};
