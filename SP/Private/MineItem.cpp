#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
    ExplosionDelay = 3.0f;  // ������ �� 3�� �� ����
    ExplosionRadius = 300.0f;
    ExplosionDamage = 30.f;
    ItemType = "Mine";
    bHasExploded = false;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);

    // ������ �̺�Ʈ �߰�
    ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &AMineItem::OnItemOverlap);
}

void AMineItem::BeginPlay()
{
    Super::BeginPlay();

    // ���� ���� �� Ÿ�̸� �ڵ� ����
    FWorldDelegates::OnWorldCleanup.AddUObject(this, &AMineItem::OnWorldCleanup);
}

void AMineItem::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
    if (World == GetWorld())
    {
        ClearTimers();
    }
}

void AMineItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (bHasExploded || !OtherActor || !OtherActor->ActorHasTag("Player")) return;

    UE_LOG(LogTemp, Warning, TEXT("Mine triggered! Exploding in %f seconds"), ExplosionDelay);

    // 3�� �� ���� Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(
        ExplosionTimerHandle,
        this,
        &AMineItem::Explode,
        ExplosionDelay,
        false
    );
}

void AMineItem::Explode()
{
    if (!IsValid(this) || !GetWorld() || bHasExploded) return;

    bHasExploded = true;

    UParticleSystemComponent* Particle = nullptr;

    if (ExplosionParticle)
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );

        if (Particle)
        {
            Particle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        }
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            ExplosionSound,
            GetActorLocation()
        );
    }

    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // ��ƼŬ ����
    TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
    TWeakObjectPtr<AMineItem> WeakThis = this; // �ڱ� �ڽ��� TWeakObjectPtr�� ����

    if (IsValid(Particle))
    {
        FTimerHandle DestroyParticleTimerHandle;

        if (IsValid(this) && GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(
                DestroyParticleTimerHandle,
                [WeakThis, WeakParticle]()
                {
                    // ������ �ٲ���ų� ������ ������ ��� �����ϰ� ����
                    if (!WeakThis.IsValid()) return;

                    if (WeakParticle.IsValid())
                    {
                        WeakParticle->DestroyComponent();
                    }

                    // ������ �����ϰ� ����
                    WeakThis->DestroyItem();
                },
                2.0f,
                false
            );
        }
    }
}

void AMineItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ClearTimers();
    UE_LOG(LogTemp, Warning, TEXT("EndPlay called for %s"), *GetName());
    DestroyItem();
}

void AMineItem::Destroyed()
{
    Super::Destroyed();
    ClearTimers();
    UE_LOG(LogTemp, Warning, TEXT("Destroyed called for %s"), *GetName());
}

void AMineItem::ClearTimers()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }
}
