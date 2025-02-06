// Fill out your copyright notice in the Description page of Project Settings.


#include "SPPawn.h"
#include "SPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASPPawn::ASPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root component (Scene component)
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Create Capsule Component
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(Root);
	CapsuleComp->InitCapsuleSize(42.f, 96.f);

	// Spring Arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(Root);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// set camera
	CameraComp->bUsePawnControlRotation = false;

	// Skeletal mesh Component
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComp); // attach SM to Capsule

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Quinn.SKM_Quinn"));

	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
		SkeletalMeshComponent->SetSimulatePhysics(false);
	}

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	RotationSpeed = 100.f;
	MinPitchAngle = -80.0f;
	MaxPitchAngle = 80.f;

}

void ASPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&ASPPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASPPawn::Look
				);
			}
		}
	}
}

void ASPPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	// 카메라 방향을 기준으로 Yaw 회전값만 가져옴
	FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

	// 이동 벡터 계산
	FVector MoveDirection = (ForwardDirection * MoveInput.X) + (RightDirection * MoveInput.Y);
	MoveDirection.Normalize(); // 대각선 이동 속도 균일화

	// 목표 위치 계산
	FVector NewLocation = GetActorLocation() + (MoveDirection * NormalSpeed * GetWorld()->GetDeltaSeconds());

	// 충돌 감지하면서 이동 적용
	SetActorLocation(NewLocation, true);

	// 카메라 방향을 기준으로 캐릭터 회전 적용
	SetActorRotation(YawRotation);
}

void ASPPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D LookInput = value.Get<FVector2D>();

	// 마우스 감도 적용
	const float MouseSensitivity = 1.5f;

	float YawDelta = LookInput.X * RotationSpeed * GetWorld()->GetDeltaSeconds() * MouseSensitivity;
	float PitchDelta = LookInput.Y * RotationSpeed * GetWorld()->GetDeltaSeconds() * MouseSensitivity;

	PitchDelta *= -1;

	FRotator NewRotation = Controller->GetControlRotation();
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + PitchDelta, MinPitchAngle, MaxPitchAngle);
	NewRotation.Yaw += YawDelta;

	Controller->SetControlRotation(NewRotation);
}