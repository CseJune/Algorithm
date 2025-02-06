// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h" // ���÷��� �ý��� ��������� �� �ڿ� include!


UCLASS() // Class�� ���÷��� �ý��ۿ� ���
class SP_API AItem : public AActor
{
	GENERATED_BODY() // ���÷��� �����͸� �ڵ����� �����ϱ� ���ؼ� �𸮾� ������ ����ϴ� ��ũ��
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

	UFUNCTION(BlueprintCallable, Category="Item|Actions")
	void ResetActorPosition();

	UFUNCTION(BlueprintPure, Category="Item|Properties")
	float GetRotationSpeed() const;

	UFUNCTION(BlueprintImplementableEvent, Category="Item|Event")
	void OnItemPickedUP();

};



// �ֿ��� ������ ����Ŭ �Լ�
// ������ - ������Ʈ�� �޸𸮿� ����� �� �ѹ� ȣ��ȴ�.
// PostInitializeComponents() - ������Ʈ�� �ϼ��� ���� ȣ��. ������Ʈ���� ������ �ְ� �ޱ�, ��ȣ�ۿ�ÿ� ����
// BegingPlay() - ��ġ(����) ���Ŀ� ȣ��ȴ�.
// Tick(float DeltaTime) - �� �����Ӹ��� ȣ��ȴ�. ������ ����Ʈ���� ������ ����� �� �����ϰ�..
// Destroyed() - ������ �Ǳ� ������ ȣ��ȴ�.
// EndPlay() - ���� ����, �ı�(Destroyed()), ���� ��ȯ