// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Movement_Actor.generated.h"

UCLASS()
class ASSIGNMENT6_API AMovement_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovement_Actor();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement_Actor|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement_Actor|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement_Actor|Properties")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement_Actor|Properties")
	float MaxDistance;

	int Direction;

	FVector InitialLocation;

	UFUNCTION(BlueprintPure, Category = "Movement_Actor|Properties")
	float GetMovementSpeed() const;

};
