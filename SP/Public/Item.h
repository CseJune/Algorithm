// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h" // 리플렉션 시스템 헤더파일은 맨 뒤에 include!


UCLASS() // Class를 리플렉션 시스템에 등록
class SP_API AItem : public AActor
{
	GENERATED_BODY() // 리플렉션 데이터를 자동으로 생성하기 위해서 언리얼 엔진이 사용하는 매크로
	
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



// 주요한 라이프 사이클 함수
// 생성자 - 오브젝트가 메모리에 생기며 딱 한번 호출된다.
// PostInitializeComponents() - 컴포넌트가 완성된 직후 호출. 컴포넌트끼리 데이터 주고 받기, 상호작용시에 유용
// BegingPlay() - 배치(스폰) 직후에 호출된다.
// Tick(float DeltaTime) - 매 프레임마다 호출된다. 성능을 떨어트리기 때문에 사용할 때 신중하게..
// Destroyed() - 삭제가 되기 직전에 호출된다.
// EndPlay() - 게임 종료, 파괴(Destroyed()), 레벨 전환