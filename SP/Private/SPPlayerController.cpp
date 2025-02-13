#include "SPPlayerController.h"
#include "SPGameState.h"
#include "SPGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASPPlayerController::ASPPlayerController()
	: bIsMainMenuVisible(false), // 초기화 추가
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	ToggleMenuAction(nullptr)
{
}

void ASPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}

			if (UIInputMappingContext)
			{
				Subsystem->AddMappingContext(UIInputMappingContext, 1);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

void ASPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Component 확인
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ToggleMenuAction)
		{
			EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &ASPPlayerController::ToggleMainMenu);
		}
	}
}

void ASPPlayerController::ToggleMainMenu()
{
	// 디버그 로그 추가
	UE_LOG(LogTemp, Warning, TEXT("Tab Key Pressed - Current Menu State: %s"), bIsMainMenuVisible ? TEXT("OPEN") : TEXT("CLOSED"));

	if (bIsMainMenuVisible)
	{
		// 메뉴가 열려 있다면 게임 화면으로 복귀
		ShowGameHUD();
	}
	else
	{
		// 메뉴가 닫혀 있다면 메뉴 화면 띄우기
		ShowMainMenu(false);
	}

	// 메뉴 상태를 반전
	bIsMainMenuVisible = !bIsMainMenuVisible;
}

UUserWidget* ASPPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASPPlayerController::ShowMainMenu(bool bIsRestart)
{
	// 기존 UI 제거
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	// 메뉴 위젯 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			// UI & 게임 입력을 동시에 받는 모드로 변경
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget()); // UI 포커스 설정
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);

			bShowMouseCursor = true;
		}

		// Start 버튼 텍스트 설정
		if (UTextBlock* StartButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			StartButtonText->SetText(FText::FromString(bIsRestart ? TEXT("Restart") : TEXT("Start")));
		}

		// End 버튼 텍스트 설정 (중복 제거)
		if (UTextBlock* EndButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText_1"))))
		{
			EndButtonText->SetText(FText::FromString(TEXT("End")));
		}

		// 게임 오버 상태일 경우 추가 처리
		if (bIsRestart)
		{
			// 애니메이션 실행
			if (UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayGameOverAnim function not found in MainMenuWidgetInstance"));
			}

			// 총 점수 업데이트
			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USPGameInstance* SPGameInstance = Cast<USPGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SPGameInstance->TotalScore)));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TotalScoreText widget not found in MainMenuWidgetInstance"));
			}
		}
	}
}

void ASPPlayerController::ShowGameHUD()
{
	// 기존 UI 제거
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	// 게임 UI 생성
	if (HUDWidgetClass && GetWorld())
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HUDWidgetInstance failed to create from HUDWidgetClass"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDWidgetClass is NULL or GetWorld() is invalid!"));
	}

	// HUD 업데이트
	if (ASPGameState* SPGameState = GetWorld() ? GetWorld()->GetGameState<ASPGameState>() : nullptr)
	{
		SPGameState->UpdateHUD();
	}
}

void ASPPlayerController::StartGame()
{
	// 레벨 초기화
	if (USPGameInstance* SPGameInstance = Cast<USPGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SPGameInstance->CurrentLevelIndex = 0; //
		SPGameInstance->TotalScore = 0;
	}
	// UI 정리
	ShowGameHUD();

	// 레벨 전환 전에 모든 타이머 정리
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	// 일시 정지 해제
	SetPause(false);

	// 레벨 시작
	if (GetWorld())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL! Cannot change level."));
	}
}

void ASPPlayerController::EndGame()
{
	// UI 정리
	ShowMainMenu(true);

	// 타이머 정리
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	// 게임 종료
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
