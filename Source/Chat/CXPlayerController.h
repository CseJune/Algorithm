// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CXPlayerController.generated.h"

class UCXChatInput;
class UUserWidget;

// 채팅 위젯, 채팅 메시지, 서버 통신 처리
UCLASS()
class CHAT_API ACXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACXPlayerController();

	virtual void BeginPlay() override;

	// 채팅 메시지 전달
	void SetChatMessageString(const FString& InChatMessageString);

	// 클라이언트 채팅 메시지 출력
	void PrintChatMessageString(const FString& InChatMessageString);

	// 서버에서 클라이언트 메시지 전송
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	// 클라이언트에서 서버로 메시지 전송
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// 채팅 입력 위젯
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCXChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCXChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	// 알림 텍스트 위젯
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	// 게임 상태 공지사항 메시지(턴, 승리 등)
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};

