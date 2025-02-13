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

	// TSubclassOf = �ϵ� ���۷���, Ŭ������ �׻� �޸𸮿� �ε�� ���¿��� �ٷ� ����, ������ ����Ŭ�������� ������ �� �ִ� ���ø�
	// TSoftClassPtr = ����Ʈ ���۷���, Ŭ������ ��θ� ����, �ش� Ŭ������ �ʿ��� ��Ȳ�� ���� �޸𸮿� �ε�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
