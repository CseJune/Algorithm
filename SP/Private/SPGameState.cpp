#include "SPGameState.h"
#include "SPPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "SPGameInstance.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASPGameState::ASPGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	// 웨이브
	CurrentWaveIndex = 0;
	MaxWavesPerLevel = 3;
}

void ASPGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASPGameState::UpdateHUD,
		0.1f,
		true
	);
}
int32 ASPGameState::GetScore() const
{
	return Score;
}

void ASPGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USPGameInstance* SPGameInstance = Cast<USPGameInstance>(GameInstance);
		if (SPGameInstance)
		{
			SPGameInstance->AddToScore(Amount);
		}
	}
}

void ASPGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPPlayerController* SPPlayerController = Cast<ASPPlayerController>(PlayerController))
		{
			SPPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USPGameInstance* SPGameInstance = Cast<USPGameInstance>(GameInstance);
		if (SPGameInstance)
		{
			CurrentLevelIndex = SPGameInstance->CurrentLevelIndex;
			// 레벨이 시작되면 웨이브 진행
			MaxWavesPerLevel = 3 + CurrentLevelIndex;  // 웨이브 개수 조정
			CurrentWaveIndex = 0;
			StartWave();  // 첫 번째 웨이브 시작
		}
	}
}


void ASPGameState::StartWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), CurrentWaveIndex + 1);

	/** 기존 아이템 삭제 */
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	/** 적 또는 코인 스폰 */
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	/** 웨이브 난이도 조정 (웨이브마다 적 개수 증가) */
	int32 ItemToSpawn = 20 + (CurrentWaveIndex * 10);

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	/** 웨이브 제한 시간 (기본 30초, 웨이브가 진행될수록 시간 증가) */
	float WaveTimeLimit = 5.0f + (CurrentWaveIndex * 1.0f);
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ASPGameState::OnWaveComplete,
		WaveTimeLimit,
		false
	);
}

/** 웨이브 종료 */
void ASPGameState::OnWaveComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Complete!"), CurrentWaveIndex + 1);

	TWeakObjectPtr<ASPGameState> WeakThis = this;

	if (GetWorld() && GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	}

	CurrentWaveIndex++;

	if (CurrentWaveIndex < MaxWavesPerLevel)
	{
		if (WeakThis.IsValid() && WeakThis->GetWorld())
		{
			// 타이머가 이미 존재하는지 다시 확인 후 실행
			if (!WeakThis->GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
			{
				WeakThis->GetWorldTimerManager().SetTimerForNextTick([WeakThis]()
					{
						if (WeakThis.IsValid() && WeakThis->GetWorld())
						{
							WeakThis->StartWave();
						}
					});
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WaveTimerHandle is still active!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ASPGameState is invalid. Skipping wave."));
		}
	}
	else
	{
		// EndLevel()이 호출되면 객체가 삭제될 가능성이 있어 WeakThis 사용
		if (WeakThis.IsValid())
		{
			WeakThis->EndLevel();
		}
	}
}


void ASPGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASPGameState::OnCoinCollected()
{
	TWeakObjectPtr<ASPGameState> WeakThis = this;

	if (!WeakThis.IsValid() || !GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("ASPGameState is invalid. Skipping coin collection."));
		return;
	}

	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		if ((CurrentWaveIndex + 1) < MaxWavesPerLevel)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->OnWaveComplete();
			}
		}
		else
		{
			if (WeakThis.IsValid())
			{
				WeakThis->EndLevel();
			}
		}
	}
}


void ASPGameState::EndLevel()
{
	TWeakObjectPtr<ASPGameState> WeakThis = this;

	if (WeakThis.IsValid() && GetWorld())
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);

		// 레벨이 바뀌기 전에 점수 및 현재 레벨 인덱스 업데이트
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (USPGameInstance* SPGameInstance = Cast<USPGameInstance>(GameInstance))
			{
				AddScore(Score);
				CurrentLevelIndex++;
				SPGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			}
		}

		// 최대 레벨을 초과하면 게임 종료 처리
		if (CurrentLevelIndex >= MaxLevels)
		{
			OnGameOver();
			return;
		}

		// 안전한 비동기 레벨 로드
		GetWorldTimerManager().SetTimerForNextTick([WeakThis]()
			{
				if (WeakThis.IsValid())
				{
					UWorld* World = WeakThis->GetWorld();
					if (World && WeakThis->LevelMapNames.IsValidIndex(WeakThis->CurrentLevelIndex))
					{
						UE_LOG(LogTemp, Warning, TEXT("Loading Level: %s"), *WeakThis->LevelMapNames[WeakThis->CurrentLevelIndex].ToString());
						UGameplayStatics::OpenLevel(World, WeakThis->LevelMapNames[WeakThis->CurrentLevelIndex]);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("GetWorld() is NULL or Invalid Level Index!"));
						if (WeakThis.IsValid())
						{
							WeakThis->OnGameOver();
						}
					}
				}
			});
	}
}




void ASPGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPPlayerController* SPPlayerController = Cast<ASPPlayerController>(PlayerController))
		{
			SPPlayerController->SetPause(true);
			SPPlayerController->ShowMainMenu(true);
		}
	}

}

void ASPGameState::UpdateHUD()
{
	if (!IsInGameThread()) // UI 업데이트를 게임 스레드에서만 실행
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHUD() was called outside of the game thread!"));
		return;
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASPPlayerController* SPPlayerController = Cast<ASPPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SPPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					if (GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
					{
						float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
						TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
					}
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USPGameInstance* SPGameInstance = Cast<USPGameInstance>(GameInstance);
						if (SPGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SPGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}


			}
		}
	}
}