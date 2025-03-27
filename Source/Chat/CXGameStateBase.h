// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameStateBase.generated.h"

// 전체 게임 상태 관리
UCLASS()
class CHAT_API ACXGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 전체 클라이언트에 로그인 메시지 전송
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
};
