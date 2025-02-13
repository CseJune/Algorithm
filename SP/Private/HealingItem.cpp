#include "HealingItem.h"
#include "SP_Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (!IsValid(Activator))  // Activator가 nullptr인지 먼저 확인
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem activated with a NULL Activator!"));
        return;
    }

    if (!Activator->ActorHasTag("Player"))  // 태그 확인 전에 Activator가 유효한지 확인
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem activated by non-player actor: %s"), *Activator->GetName());
        return;
    }

    // 안전한 캐스팅
    ASP_Character* PlayerCharacter = Cast<ASP_Character>(Activator);
    if (!IsValid(PlayerCharacter))  // 캐스팅 실패 시 처리
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem could not cast Activator to ASP_Character"));
        return;
    }

    // 유효한 플레이어 캐릭터에게 체력 추가
    PlayerCharacter->AddHealth(HealAmount);
    UE_LOG(LogTemp, Log, TEXT("HealingItem healed player: %s for %d health"), *PlayerCharacter->GetName(), HealAmount);

    DestroyItem();  // 힐링 아이템 제거
}
