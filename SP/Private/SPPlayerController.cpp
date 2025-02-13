#include "SPPlayerController.h"
#include "SPGameState.h"
#include "SPGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ASPPlayerController::ASPPlayerController()
	: bIsMainMenuVisible(false), // �ʱ�ȭ �߰�
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

	// Enhanced Input Component Ȯ��
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
	// ����� �α� �߰�
	UE_LOG(LogTemp, Warning, TEXT("Tab Key Pressed - Current Menu State: %s"), bIsMainMenuVisible ? TEXT("OPEN") : TEXT("CLOSED"));

	if (bIsMainMenuVisible)
	{
		// �޴��� ���� �ִٸ� ���� ȭ������ ����
		ShowGameHUD();
	}
	else
	{
		// �޴��� ���� �ִٸ� �޴� ȭ�� ����
		ShowMainMenu(false);
	}

	// �޴� ���¸� ����
	bIsMainMenuVisible = !bIsMainMenuVisible;
}

UUserWidget* ASPPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASPPlayerController::ShowMainMenu(bool bIsRestart)
{
	// ���� UI ����
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

	// �޴� ���� ����
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			// UI & ���� �Է��� ���ÿ� �޴� ���� ����
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget()); // UI ��Ŀ�� ����
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);

			bShowMouseCursor = true;
		}

		// Start ��ư �ؽ�Ʈ ����
		if (UTextBlock* StartButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			StartButtonText->SetText(FText::FromString(bIsRestart ? TEXT("Restart") : TEXT("Start")));
		}

		// End ��ư �ؽ�Ʈ ���� (�ߺ� ����)
		if (UTextBlock* EndButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText_1"))))
		{
			EndButtonText->SetText(FText::FromString(TEXT("End")));
		}

		// ���� ���� ������ ��� �߰� ó��
		if (bIsRestart)
		{
			// �ִϸ��̼� ����
			if (UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayGameOverAnim function not found in MainMenuWidgetInstance"));
			}

			// �� ���� ������Ʈ
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
	// ���� UI ����
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

	// ���� UI ����
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

	// HUD ������Ʈ
	if (ASPGameState* SPGameState = GetWorld() ? GetWorld()->GetGameState<ASPGameState>() : nullptr)
	{
		SPGameState->UpdateHUD();
	}
}

void ASPPlayerController::StartGame()
{
	// ���� �ʱ�ȭ
	if (USPGameInstance* SPGameInstance = Cast<USPGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SPGameInstance->CurrentLevelIndex = 0; //
		SPGameInstance->TotalScore = 0;
	}
	// UI ����
	ShowGameHUD();

	// ���� ��ȯ ���� ��� Ÿ�̸� ����
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	// �Ͻ� ���� ����
	SetPause(false);

	// ���� ����
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
	// UI ����
	ShowMainMenu(true);

	// Ÿ�̸� ����
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	// ���� ����
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
