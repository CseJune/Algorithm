// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerState.generated.h"

//�÷��̾� �̸��� �õ� Ƚ�� ���� ���¸� �����ϴ� Ŭ����
UCLASS()
class CHAT_API ACXPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACXPlayerState();

	// ��Ʈ��ũ�� ������ ���� ���
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// UI ǥ�ÿ� ���ڿ� ��ȯ
	FString GetPlayerInfoString();

public:
	// �÷��̾� �̸�
	UPROPERTY(Replicated)
	FString PlayerNameString;

	// ���� �õ� Ƚ��
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	// �ִ� �õ� Ƚ��
	UPROPERTY(Replicated)
	int32 MaxGuessCount;

};
