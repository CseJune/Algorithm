#include "HealingItem.h"
#include "SP_Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (!IsValid(Activator))  // Activator�� nullptr���� ���� Ȯ��
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem activated with a NULL Activator!"));
        return;
    }

    if (!Activator->ActorHasTag("Player"))  // �±� Ȯ�� ���� Activator�� ��ȿ���� Ȯ��
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem activated by non-player actor: %s"), *Activator->GetName());
        return;
    }

    // ������ ĳ����
    ASP_Character* PlayerCharacter = Cast<ASP_Character>(Activator);
    if (!IsValid(PlayerCharacter))  // ĳ���� ���� �� ó��
    {
        UE_LOG(LogTemp, Warning, TEXT("HealingItem could not cast Activator to ASP_Character"));
        return;
    }

    // ��ȿ�� �÷��̾� ĳ���Ϳ��� ü�� �߰�
    PlayerCharacter->AddHealth(HealAmount);
    UE_LOG(LogTemp, Log, TEXT("HealingItem healed player: %s for %d health"), *PlayerCharacter->GetName(), HealAmount);

    DestroyItem();  // ���� ������ ����
}
