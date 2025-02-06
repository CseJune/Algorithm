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
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // 피치, 요, 롤(Y축기준, Z축기준, X축기준)
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

// 주요한 라이프 사이클 함수

// 생성자 - 오브젝트가 메모리에 생기며 딱 한번 호출된다.
// PostInitializeComponents() - 컴포넌트가 완성된 직후 호출. 컴포넌트끼리 데이터 주고 받기, 상호작용시에 유용
// BegingPlay() - 배치(스폰) 직후에 호출된다.
// Tick(float DeltaTime) - 매 프레임마다 호출된다. 성능을 떨어트리기 때문에 사용할 때 신중하게..
// Destroyed() - 삭제가 되기 직전에 호출된다.
// EndPlay() - 게임 종료, 파괴(Destroyed()), 레벨 전환