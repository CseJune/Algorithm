// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CXPlayerController.generated.h"

class UCXChatInput;
class UUserWidget;

// ä�� ����, ä�� �޽���, ���� ��� ó��
UCLASS()
class CHAT_API ACXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACXPlayerController();

	virtual void BeginPlay() override;

	// ä�� �޽��� ����
	void SetChatMessageString(const FString& InChatMessageString);

	// Ŭ���̾�Ʈ ä�� �޽��� ���
	void PrintChatMessageString(const FString& InChatMessageString);

	// �������� Ŭ���̾�Ʈ �޽��� ����
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	// Ŭ���̾�Ʈ���� ������ �޽��� ����
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// ä�� �Է� ����
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCXChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UCXChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	// �˸� �ؽ�Ʈ ����
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	// ���� ���� �������� �޽���(��, �¸� ��)
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};

