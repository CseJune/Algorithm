#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class SP_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()
	
public:
	ABigCoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
