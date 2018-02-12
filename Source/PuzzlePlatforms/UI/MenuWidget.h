// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class IMenuInterface;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Setup();
	virtual void TearDown();
	virtual void SetMenuInterface(IMenuInterface* menuInterface);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* inLevel, UWorld* inWorld) override;

protected:
	IMenuInterface * m_menuInterface;
	
};
