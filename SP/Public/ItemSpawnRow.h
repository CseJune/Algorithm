#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	// TSubclassOf = 하드 레퍼런스, 클래스가 항상 메모리에 로드된 상태에서 바로 접근, 액터의 하위클래스까지 포괄할 수 있는 템플릿
	// TSoftClassPtr = 소프트 레퍼런스, 클래스의 경로만 유지, 해당 클래스가 필요한 상황일 때만 메모리에 로드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
