// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class FOnlineSessionSearch;
class IOnlineSubsystem;
class UUserWidget;
class UMainMenu;
class UMenuWidget;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> WBP_MainMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> WBP_InGameMenu;

	UPROPERTY(EditAnywhere)
	TAssetPtr<UWorld> LevelToHost;

public:
	UPuzzlePlatformsGameInstance();

	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void HostServer() override;

	UFUNCTION(Exec)
	void JoinServer(const FString& address) override;

	UFUNCTION(Exec)
	void QuitServer() override;

	UFUNCTION(Exec)
	void QuitGame() override;

	UFUNCTION(Exec)
	void RefreshServerList() override;

private:
	void onCreateSessionCompleted(FName sessionName, bool success);
	void onDestroySessionCompleted(FName sessionName, bool success);
	void onFindSessionsCompleted(bool success);
	void createNewSession();

	UMainMenu* m_mainMenu = nullptr;
	IOnlineSubsystem* m_onlineSubsystem = nullptr;
	FName m_sessionName = "MySession";
	TSharedPtr<FOnlineSessionSearch> m_sessionSearch;
};
