// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rotation_Actor.generated.h"

UCLASS()
class ASSIGNMENT6_API ARotation_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotation_Actor();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation_Actor|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation_Actor|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation_Actor|Properties")
	float RotationSpeed;

	UFUNCTION(BlueprintPure, Category = "Rotation_Actor|Properties")
	float GetRotationSpeed() const;

};
