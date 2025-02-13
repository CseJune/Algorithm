#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SPGameInstance.generated.h"

UCLASS()
class SP_API USPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USPGameInstance();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "Gamedata")
	void AddToScore(int32 Amount);
};
