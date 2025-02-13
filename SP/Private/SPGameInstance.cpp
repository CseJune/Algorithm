#include "SPGameInstance.h"

USPGameInstance::USPGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void USPGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}