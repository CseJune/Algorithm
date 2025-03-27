// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGameModeBase.generated.h"

class ACXPlayerController;

// 게임 전반 흐름 제어
// 플레이어 접속, 턴 관리, 정답 생성 및 판별, 채팅 메시지 처리
UCLASS()
class CHAT_API ACXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// 플레이어 로그인 시 호출
	virtual void OnPostLogin(AController* NewPlayer) override;

	// 세 자리 비중복 난수 생성 함수
	FString GenerateSecretNumber();

	// 3자리 숫자가 유효한 입력인지 확인 (중복 불가, 0 불가)
	bool IsGuessNumberString(const FString& InNumberString);

	// 정답과 입력한 값 비교하여 결과 반환
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	// 채팅 메시지 분석 및 전달
	void PrintChatMessageString(ACXPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	// 플레이어의 추측 횟수 증가
	void IncreaseGuessCount(ACXPlayerController* InChattingPlayerController);

	// 게임 초기화 (정답 재생성, 턴 초기화, 시도 횟수 초기화)
	void ResetGame();

	// 정답 맞추었거나 무승부 판단
	void JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount);

	// 현재 턴인 플레이어 반환
	ACXPlayerController* GetCurrentTurnPlayerController() const;

private:
	// 타이머 콜백: 턴 전환 처리
	UFUNCTION()
	void OnMainTimerElapsed();

protected:
	// 정답 숫자
	FString SecretNumberString;

	// 모든 접속 플레이어 목록
	TArray<TObjectPtr<ACXPlayerController>> AllPlayerControllers;

	// 메인 타이머 핸들 (턴 관리용)
	FTimerHandle MainTimerHandle;

	// 현재 턴 인덱스
	int32 CurrentTurnIndex = 0;
};
