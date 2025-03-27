// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameStateBase.generated.h"

// ��ü ���� ���� ����
UCLASS()
class CHAT_API ACXGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// ��ü Ŭ���̾�Ʈ�� �α��� �޽��� ����
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
};
