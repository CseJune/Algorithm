// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerState.generated.h"

//플레이어 이름과 시도 횟수 등의 상태를 보관하는 클래스
UCLASS()
class CHAT_API ACXPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACXPlayerState();

	// 네트워크에 복제할 변수 등록
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// UI 표시용 문자열 변환
	FString GetPlayerInfoString();

public:
	// 플레이어 이름
	UPROPERTY(Replicated)
	FString PlayerNameString;

	// 현재 시도 횟수
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	// 최대 시도 횟수
	UPROPERTY(Replicated)
	int32 MaxGuessCount;

};
