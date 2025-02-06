// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Chair.SM_Chair"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}
	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	OnItemPickedUP();

	SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // ��ġ, ��, ��(Y�����, Z�����, X�����)
	SetActorScale3D(FVector(2.0f));

}

void AItem::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * Deltatime, 0.0f));

	}
}

void AItem::ResetActorPosition()
{
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed() const
{
	return RotationSpeed;
}

// �ֿ��� ������ ����Ŭ �Լ�

// ������ - ������Ʈ�� �޸𸮿� ����� �� �ѹ� ȣ��ȴ�.
// PostInitializeComponents() - ������Ʈ�� �ϼ��� ���� ȣ��. ������Ʈ���� ������ �ְ� �ޱ�, ��ȣ�ۿ�ÿ� ����
// BegingPlay() - ��ġ(����) ���Ŀ� ȣ��ȴ�.
// Tick(float DeltaTime) - �� �����Ӹ��� ȣ��ȴ�. ������ ����Ʈ���� ������ ����� �� �����ϰ�..
// Destroyed() - ������ �Ǳ� ������ ȣ��ȴ�.
// EndPlay() - ���� ����, �ı�(Destroyed()), ���� ��ȯ