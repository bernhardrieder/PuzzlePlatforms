// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* m_quitServerButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* m_backToGameButton = nullptr;

public:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void quitServerBtnClicked();

	UFUNCTION()
	void backToGameBtnClicked();
};
