// Fill out your copyright notice in the Description page of Project Settings.


#include "Rotation_Actor.h"

// Sets default values
ARotation_Actor::ARotation_Actor()
{



	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	MeshAsset(TEXT("/Game/StarterContent/Props/SM_Shelf.SM_Shelf"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/StarterContent/Materials/M_Metal_Chrome.M_Metal_Chrome"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 120.0f;

}

// Called when the game starts or when spawned
void ARotation_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotation_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, 0.0f, RotationSpeed * DeltaTime));
	}
}

float ARotation_Actor::GetRotationSpeed() const
{
	return RotationSpeed;
}