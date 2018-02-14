// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "OnlineSessionInterface.h"
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

public:
	UPROPERTY(EditAnywhere)
	TAssetPtr<UWorld> LobbyLevel;

public:
	UPuzzlePlatformsGameInstance();

	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void HostServer(const FString& serverName) override;

	UFUNCTION(Exec)
	void JoinServer(int32 serverIndex) override;

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
	void onJoinSessionCompleted(FName sessionName, EOnJoinSessionCompleteResult::Type bWasSuccessful);

	UMainMenu* m_mainMenu = nullptr;
	IOnlineSubsystem* m_onlineSubsystem = nullptr;
	IOnlineSessionPtr m_sessionInterface;
	FName m_sessionName = "MySession";
	TSharedPtr<FOnlineSessionSearch> m_sessionSearch;
	FString m_serverName;
	static FName s_serverNameKey;

};
