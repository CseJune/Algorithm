// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement_Actor.h"

// Sets default values
AMovement_Actor::AMovement_Actor()
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
	MovementSpeed = 200.0f;
	MaxDistance = 2000.f;
	Direction = 1;
	InitialLocation = GetActorLocation();

}

// Called when the game starts or when spawned
void AMovement_Actor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMovement_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MovementSpeed))
	{
		FVector NewLocation = GetActorLocation();
		float DistanceMoved = FVector::Distance(NewLocation, InitialLocation);

		if (DistanceMoved >= MaxDistance)
		{
			Direction *= -1;
			InitialLocation = NewLocation;
		}
		AddActorWorldOffset(FVector(0.0f, MovementSpeed * DeltaTime * Direction, 0.0f));
	}
}

float AMovement_Actor::GetMovementSpeed() const
{
	return MovementSpeed;
}