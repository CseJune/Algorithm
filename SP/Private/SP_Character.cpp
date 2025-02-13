#include "SP_Character.h"
#include "SPPlayerController.h"
#include "SPGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ASP_Character::ASP_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100;
	Health = MaxHealth;
}

void ASP_Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHp();

}


void ASP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASPPlayerController* PlayerController = Cast<ASPPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASP_Character::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASP_Character::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASP_Character::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASP_Character::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASP_Character::StartSprint
				);

				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASP_Character::StopSprint
				);
			}
		}
	}
}

void ASP_Character::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASP_Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASP_Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASP_Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}
void ASP_Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASP_Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float ASP_Character::GetHealth() const
{
	return Health;
}

void ASP_Character::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth); // FMath::Clamp�� �̿��� �ִ�ü���� �Ѿ�� ���ϰ� 0���� �������� ���ϰ� ����
	UpdateOverheadHp();
}

float ASP_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ������ ���� ǥ��
	ShowDamageText(DamageAmount);

	// ü�� ����
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHp();

	// ��� üũ
	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASP_Character::OnDeath()
{
	ASPGameState* SPGameState = GetWorld() ? GetWorld()->GetGameState<ASPGameState>() : nullptr;
	if (SPGameState)
	{
		SPGameState->OnGameOver();
	}
}

void ASP_Character::UpdateOverheadHp()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		// ü�� �ؽ�Ʈ ������Ʈ
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));

		// ü���� 30 ���϶�� ������ ���������� ����
		if (Health <= 30.0f)
		{
			HPText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		}
		else
		{
			HPText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		}
	}
}

void ASP_Character::ShowDamageText(float DamageAmount)
{
	// UI Ŭ������ �����ϴ��� Ȯ��
	if (!DamageTextClass || !GetWorld()) return;

	// ������ UI ����
	UUserWidget* DamageWidget = CreateWidget<UUserWidget>(GetWorld(), DamageTextClass);
	if (DamageWidget)
	{
		DamageWidget->AddToViewport();

		// TextBlock ����
		if (UTextBlock* DamageText = Cast<UTextBlock>(DamageWidget->GetWidgetFromName(TEXT("DamageText"))))
		{
			DamageText->SetText(FText::FromString(FString::Printf(TEXT("-%.0f"), DamageAmount)));
		}

		// �ִϸ��̼� ���� (�ִϸ��̼� �̸�: "FadeOut")
		UFunction* PlayAnimFunc = DamageWidget->FindFunction(FName("PlayFadeOut"));
		if (PlayAnimFunc)
		{
			DamageWidget->ProcessEvent(PlayAnimFunc, nullptr);
		}

		// 1�� �� ����
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DamageWidget]()
			{
				DamageWidget->RemoveFromParent();
			}, 1.0f, false);
	}
}