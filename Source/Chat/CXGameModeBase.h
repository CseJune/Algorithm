// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGameModeBase.generated.h"

class ACXPlayerController;

// ���� ���� �帧 ����
// �÷��̾� ����, �� ����, ���� ���� �� �Ǻ�, ä�� �޽��� ó��
UCLASS()
class CHAT_API ACXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// �÷��̾� �α��� �� ȣ��
	virtual void OnPostLogin(AController* NewPlayer) override;

	// �� �ڸ� ���ߺ� ���� ���� �Լ�
	FString GenerateSecretNumber();

	// 3�ڸ� ���ڰ� ��ȿ�� �Է����� Ȯ�� (�ߺ� �Ұ�, 0 �Ұ�)
	bool IsGuessNumberString(const FString& InNumberString);

	// ����� �Է��� �� ���Ͽ� ��� ��ȯ
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	// ä�� �޽��� �м� �� ����
	void PrintChatMessageString(ACXPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
	// �÷��̾��� ���� Ƚ�� ����
	void IncreaseGuessCount(ACXPlayerController* InChattingPlayerController);

	// ���� �ʱ�ȭ (���� �����, �� �ʱ�ȭ, �õ� Ƚ�� �ʱ�ȭ)
	void ResetGame();

	// ���� ���߾��ų� ���º� �Ǵ�
	void JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount);

	// ���� ���� �÷��̾� ��ȯ
	ACXPlayerController* GetCurrentTurnPlayerController() const;

private:
	// Ÿ�̸� �ݹ�: �� ��ȯ ó��
	UFUNCTION()
	void OnMainTimerElapsed();

protected:
	// ���� ����
	FString SecretNumberString;

	// ��� ���� �÷��̾� ���
	TArray<TObjectPtr<ACXPlayerController>> AllPlayerControllers;

	// ���� Ÿ�̸� �ڵ� (�� ������)
	FTimerHandle MainTimerHandle;

	// ���� �� �ε���
	int32 CurrentTurnIndex = 0;
};
