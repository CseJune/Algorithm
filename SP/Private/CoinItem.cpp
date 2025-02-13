#include "CoinItem.h"
#include "Engine/World.h"
#include "SPGameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaulteCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASPGameState* GameState = World->GetGameState<ASPGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}

		DestroyItem();
	}
}