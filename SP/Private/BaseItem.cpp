#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAlldynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	// �̺�Ʈ ���ε� �ſ� �߿�
	// ������ �̺�Ʈ �߻� -> ��� �̺�Ʈ�� �߻��ϴ°�?
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

void ABaseItem::OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
{

}
void ABaseItem::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle && GetWorld())
	{
		//  Ÿ�̸� ���� ���� ��ȿ�� üũ
		FTimerHandle DestroyParticleTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle = TWeakObjectPtr<UParticleSystemComponent>(Particle)]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f,
			false
		);

		ClearTimers();
		DestroyItem();
	}
}
FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

void ABaseItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearTimers(); // Ÿ�̸� ����
	UE_LOG(LogTemp, Warning, TEXT("EndPlay called for %s"), *GetName());
}

void ABaseItem::Destroyed()
{
	Super::Destroyed();
	ClearTimers(); // Ÿ�̸� ����
	UE_LOG(LogTemp, Warning, TEXT("OnDestroyed called for %s"), *GetName());
}


void ABaseItem::ClearTimers()
{
	if (GetWorld())
	{
		// ����� ��� Ÿ�̸� ����
		for (FTimerHandle Timer : ActiveTimers)
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer);
		}
	}
}